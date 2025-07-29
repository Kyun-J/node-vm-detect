#include <napi.h>
#include "vmaware.hpp"

class VMDetectWorker : public Napi::AsyncWorker {
private:
    VM::vmaware vm;
    std::string errorMessage;
    Napi::Promise::Deferred deferred;

public:
    VMDetectWorker(Napi::Env env) : Napi::AsyncWorker(env), deferred(Napi::Promise::Deferred::New(env)) {}

    Napi::Promise GetPromise() {
        return deferred.Promise();
    }

    void Execute() override {
        try {
            vm = VM::vmaware();
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
            result.Set("techniqueCount", Napi::Number::New(env, vm.technique_count));

            deferred.Resolve(result);
        }
    }

    void OnError(const Napi::Error& e) override {
        deferred.Reject(e.Value());
    }
};

Napi::Value GetVMInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    VMDetectWorker* worker = new VMDetectWorker(env);
    worker->Queue();

    return worker->GetPromise();
}

void InitGetVMInfo(Napi::Env env, Napi::Object exports) {
    exports.Set("getVMInfo", Napi::Function::New(env, GetVMInfo));
}
