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
        // Windows
        {"GPU_CAPABILITIES", VM::GPU_CAPABILITIES},
        {"TPM", VM::TPM},
        {"ACPI_SIGNATURE", VM::ACPI_SIGNATURE},
        {"POWER_CAPABILITIES", VM::POWER_CAPABILITIES},
        {"DISK_SERIAL", VM::DISK_SERIAL},
        {"IVSHMEM", VM::IVSHMEM},
        {"SGDT", VM::SGDT},
        {"SLDT", VM::SLDT},
        {"SMSW", VM::SMSW},
        {"DRIVERS", VM::DRIVERS},
        {"REGISTRY_VALUES", VM::REGISTRY_VALUES},
        {"LOGICAL_PROCESSORS", VM::LOGICAL_PROCESSORS},
        {"PHYSICAL_PROCESSORS", VM::PHYSICAL_PROCESSORS},
        {"DEVICE_HANDLES", VM::DEVICE_HANDLES},
        {"VIRTUAL_PROCESSORS", VM::VIRTUAL_PROCESSORS},
        {"HYPERV_QUERY", VM::HYPERV_QUERY},
        {"REGISTRY_KEYS", VM::REGISTRY_KEYS},
        {"AUDIO", VM::AUDIO},
        {"DISPLAY", VM::DISPLAY},
        {"DLL", VM::DLL},
        {"VMWARE_BACKDOOR", VM::VMWARE_BACKDOOR},
        {"WINE", VM::WINE},
        {"VIRTUAL_REGISTRY", VM::VIRTUAL_REGISTRY},
        {"MUTEX", VM::MUTEX},
        {"DEVICE_STRING", VM::DEVICE_STRING},
        {"VPC_INVALID", VM::VPC_INVALID},
        {"VMWARE_STR", VM::VMWARE_STR},
        {"GAMARUE", VM::GAMARUE},
        {"CUCKOO_DIR", VM::CUCKOO_DIR},
        {"CUCKOO_PIPE", VM::CUCKOO_PIPE},
        {"TRAP", VM::TRAP},
        {"UD", VM::UD},
        {"BLOCKSTEP", VM::BLOCKSTEP},
        {"DBVM", VM::DBVM},
        {"BOOT_LOGO", VM::BOOT_LOGO},

        // Linux and Windows
        {"SIDT", VM::SIDT},
        {"FIRMWARE", VM::FIRMWARE},
        {"PCI_DEVICES", VM::PCI_DEVICES},
        {"DISK_SIZE", VM::DISK_SIZE},
        {"HYPERV_HOSTNAME", VM::HYPERV_HOSTNAME},
        {"GENERAL_HOSTNAME", VM::GENERAL_HOSTNAME},
        {"VBOX_DEFAULT", VM::VBOX_DEFAULT},

        // Linux
        {"SMBIOS_VM_BIT", VM::SMBIOS_VM_BIT},
        {"KMSG", VM::KMSG},
        {"CVENDOR", VM::CVENDOR},
        {"QEMU_FW_CFG", VM::QEMU_FW_CFG},
        {"SYSTEMD", VM::SYSTEMD},
        {"CTYPE", VM::CTYPE},
        {"DOCKERENV", VM::DOCKERENV},
        {"DMIDECODE", VM::DMIDECODE},
        {"DMESG", VM::DMESG},
        {"HWMON", VM::HWMON},
        {"LINUX_USER_HOST", VM::LINUX_USER_HOST},
        {"VMWARE_IOMEM", VM::VMWARE_IOMEM},
        {"VMWARE_IOPORTS", VM::VMWARE_IOPORTS},
        {"VMWARE_SCSI", VM::VMWARE_SCSI},
        {"VMWARE_DMESG", VM::VMWARE_DMESG},
        {"QEMU_VIRTUAL_DMI", VM::QEMU_VIRTUAL_DMI},
        {"QEMU_USB", VM::QEMU_USB},
        {"HYPERVISOR_DIR", VM::HYPERVISOR_DIR},
        {"UML_CPU", VM::UML_CPU},
        {"VBOX_MODULE", VM::VBOX_MODULE},
        {"SYSINFO_PROC", VM::SYSINFO_PROC},
        {"DMI_SCAN", VM::DMI_SCAN},
        {"PODMAN_FILE", VM::PODMAN_FILE},
        {"WSL_PROC", VM::WSL_PROC},
        {"FILE_ACCESS_HISTORY", VM::FILE_ACCESS_HISTORY},
        {"MAC", VM::MAC},
        {"NSJAIL_PID", VM::NSJAIL_PID},
        {"BLUESTACKS_FOLDERS", VM::BLUESTACKS_FOLDERS},
        {"AMD_SEV", VM::AMD_SEV},
        {"TEMPERATURE", VM::TEMPERATURE},
        {"PROCESSES", VM::PROCESSES},

        // Linux and MacOS
        {"THREAD_COUNT", VM::THREAD_COUNT},

        // MacOS
        {"MAC_MEMSIZE", VM::MAC_MEMSIZE},
        {"MAC_IOKIT", VM::MAC_IOKIT},
        {"MAC_SIP", VM::MAC_SIP},
        {"IOREG_GREP", VM::IOREG_GREP},
        {"HWMODEL", VM::HWMODEL},
        {"MAC_UTM", VM::MAC_UTM},

        // cross-platform
        {"HYPERVISOR_BIT", VM::HYPERVISOR_BIT},
        {"VMID", VM::VMID},
        {"INTEL_THREAD_MISMATCH", VM::INTEL_THREAD_MISMATCH},
        {"AMD_THREAD_MISMATCH", VM::AMD_THREAD_MISMATCH},
        {"XEON_THREAD_MISMATCH", VM::XEON_THREAD_MISMATCH},
        {"TIMER", VM::TIMER},
        {"CPU_BRAND", VM::CPU_BRAND},
        {"HYPERVISOR_STR", VM::HYPERVISOR_STR},
        {"CPUID_SIGNATURE", VM::CPUID_SIGNATURE},
        {"ODD_CPU_THREADS", VM::ODD_CPU_THREADS},
        {"BOCHS_CPU", VM::BOCHS_CPU},
        {"KGT_SIGNATURE", VM::KGT_SIGNATURE},
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
