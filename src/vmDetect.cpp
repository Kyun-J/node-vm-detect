#include <napi.h>
#include "vmaware.hpp"

struct VMOptionsConverter {
private:
    std::map<std::string, VM::enum_flags> coreFlagMap =
    {
        {"ALL", VM::ALL},
        {"DEFAULT", VM::DEFAULT},
    };

    std::map<std::string, VM::enum_flags> settingFlagMap =
    {
        {"NO_MEMO", VM::NO_MEMO},
        {"MULTIPLE", VM::MULTIPLE},
        {"HIGH_THRESHOLD", VM::HIGH_THRESHOLD},
        {"DYNAMIC", VM::DYNAMIC},
    };

    std::map<std::string, VM::enum_flags> techniqueFlagMap =
    {
        {"VMID", VM::VMID},
        {"CPU_BRAND", VM::CPU_BRAND},
    };

public:
    using vmFlagset = std::bitset<VM::MULTIPLE + 1>;

    VM::enum_flags core = VM::DEFAULT;
    vmFlagset disableFlags;

    VMOptionsConverter(std::optional<Napi::Object> options) {
        disableFlags.set();

        if (!options || !options.has_value()) {
            return;
        }

        auto optionsObject = options.value();

        if (optionsObject.Get("core").IsString()) {
            Napi::String settingStr = optionsObject.Get("core").As<Napi::String>();
            auto it = settingFlagMap.find(settingStr);
            if (it != settingFlagMap.end()) {
                core = it->second;
            }
        }

        if (optionsObject.Get("disable").IsArray()) {
            Napi::Array disableArray = optionsObject.Get("disable").As<Napi::Array>();
            auto length = disableArray.Length();
            for (uint32_t i = 0; i < length; i++) {
                Napi::Value flagValue = disableArray[i];
                if (flagValue.IsString()) {
                    std::string flagStr = flagValue.As<Napi::String>();
                    auto it = techniqueFlagMap.find(flagStr);
                    if (it != techniqueFlagMap.end()) {
                        disableFlags.set(it->second, false);
                    }
                }
            }
        }
    }
};

class VMDetectWorker : public Napi::AsyncWorker {
private:
    VM::vmaware vm;
    std::string errorMessage;
    Napi::Promise::Deferred deferred;
    VMOptionsConverter optionsConverter;

public:
    VMDetectWorker(Napi::Env env, Napi::Promise::Deferred deferred, VMOptionsConverter optionsConverter)
        : Napi::AsyncWorker(env), deferred(deferred), optionsConverter(optionsConverter) {}

    Napi::Promise GetPromise() {
        return deferred.Promise();
    }

    void Execute() override {
        try {
            vm = VM::vmaware(optionsConverter.core, optionsConverter.disableFlags);
        } catch (const std::exception& e) {
            errorMessage = e.what();
        } catch (...) {
            errorMessage = "Unknown error occurred.";
        }
    }

    void OnOK() override {
        Napi::Env env = Env();
        Napi::HandleScope scope(env);

        if (!errorMessage.empty()) {
            Napi::Error error = Napi::Error::New(env, errorMessage);
            deferred.Reject(error.Value());
        } else {
            Napi::Object result = Napi::Object::New(env);

            result.Set("isVM", Napi::Boolean::New(env, vm.is_vm));
            result.Set("brand", Napi::String::New(env, vm.brand));
            result.Set("type", Napi::String::New(env, vm.type));
            result.Set("conclusion", Napi::String::New(env, vm.conclusion));
            result.Set("percentage", Napi::Number::New(env, vm.percentage));
            result.Set("detectedCount", Napi::Number::New(env, vm.detected_count));

            deferred.Resolve(result);
        }
    }

    void OnError(const Napi::Error& e) override {
        deferred.Reject(e.Value());
    }
};

Napi::Value GetVMInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    auto deferred = Napi::Promise::Deferred::New(env);

    try {
        std::optional<Napi::Object> options = std::nullopt;
        if (info.Length() > 0 && info[0].IsObject()) {
            options = info[0].As<Napi::Object>();
        }

        VMDetectWorker* worker = new VMDetectWorker(env, deferred, VMOptionsConverter(options));
        worker->Queue();
    } catch (const std::exception& e) {
        deferred.Reject(Napi::Error::New(env, e.what()).Value());
    } catch (...) {
        deferred.Reject(Napi::Error::New(env, "Unknown error occurred.").Value());
    }

    return deferred.Promise();
}

void InitGetVMInfo(Napi::Env env, Napi::Object exports) {
    exports.Set("getVMInfo", Napi::Function::New(env, GetVMInfo));
}
