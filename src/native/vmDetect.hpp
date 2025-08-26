#include <napi.h>
#include "vmaware.hpp"
#include "napiActive.hpp"

struct VMDetect {
private:

    struct VMOptionsParser {
    private:
        using vmFlagset = std::bitset<VM::MULTIPLE + 1>;

        std::map<std::string, VM::enum_flags> presetFlagMap =
        {
            {"ALL", VM::ALL},
            {"DEFAULT", VM::DEFAULT},
        };

        std::map<std::string, VM::enum_flags> settingFlagMap =
        {
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
            {"MAC_SYS", VM::MAC_SYS},

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
        VM::enum_flags preset = VM::DEFAULT;
        VM::enum_flags multiple = VM::DEFAULT;
        VM::enum_flags highThreshold = VM::DEFAULT;
        VM::enum_flags dynamic = VM::DEFAULT;
        vmFlagset techniqueFlags;

        bool optionExists = false;

        VMOptionsParser(const Napi::CallbackInfo& info) {
            techniqueFlags.set();

            Napi::Object options;
            if (info.Length() > 0 && info[0].IsObject()) {
                options = info[0].As<Napi::Object>();
            } else {
                return;
            }

            if (options.Get("preset").IsString()) {
                auto presetStr = options.Get("preset").As<Napi::String>();
                auto it = presetFlagMap.find(presetStr);
                if (it != presetFlagMap.end()) {
                    optionExists = true;
                    preset = it->second;
                    multiple = it->second;
                    highThreshold = it->second;
                    dynamic = it->second;
                }
            }

            if (options.Get("settings").IsArray()) {
                auto settingsArray = options.Get("settings").As<Napi::Array>();
                auto length = settingsArray.Length();
                for (uint8_t i = 0; i < length; i++) {
                    Napi::Value flagValue = settingsArray[i];
                    if (!flagValue.IsString()) {
                        continue;
                    }
                    auto flagStr = flagValue.As<Napi::String>();
                    auto it = settingFlagMap.find(flagStr);
                    switch (it->second) {
                        case VM::MULTIPLE:
                            if (multiple == it->second) {
                                continue;
                            }
                            optionExists = true;
                            multiple = it->second;
                            highThreshold = it->second;
                            dynamic = it->second;
                            break;
                        case VM::HIGH_THRESHOLD:
                            if (highThreshold == it->second) {
                                continue;
                            }
                            optionExists = true;
                            highThreshold = it->second;
                            dynamic = it->second;
                            break;
                        case VM::DYNAMIC:
                            if (dynamic == it->second) {
                                continue;
                            }
                            optionExists = true;
                            dynamic = it->second;
                            break;
                        default:
                            break;
                    }
                }
            }

            if (options.Get("techniques").IsObject()) {
                auto techniquesObject = options.Get("techniques").As<Napi::Object>();

                if (techniquesObject.Get("only").IsArray()) {
                    auto onlyArray = techniquesObject.Get("only").As<Napi::Array>();
                    auto length = onlyArray.Length();
                    bool reset = false;
                    for (uint8_t i = 0; i < length; i++) {
                        Napi::Value flagValue = onlyArray[i];
                        if (flagValue.IsString()) {
                            auto flagStr = flagValue.As<Napi::String>();
                            auto it = techniqueFlagMap.find(flagStr);
                            if (it != techniqueFlagMap.end()) {
                                optionExists = true;
                                if (!reset) {
                                    techniqueFlags.reset();
                                    reset = true;
                                }
                                techniqueFlags.set(it->second);
                            }
                        }
                    }
                }

                if (techniquesObject.Get("disable").IsArray()) {
                    auto disableArray = techniquesObject.Get("disable").As<Napi::Array>();
                    auto length = disableArray.Length();
                    for (uint8_t i = 0; i < length; i++) {
                        Napi::Value flagValue = disableArray[i];
                        if (flagValue.IsString()) {
                            auto flagStr = flagValue.As<Napi::String>();
                            auto it = techniqueFlagMap.find(flagStr);
                            if (it != techniqueFlagMap.end()) {
                                optionExists = true;
                                techniqueFlags.set(it->second, false);
                            }
                        }
                    }
                }
            }
        }
    };

    enum VMDetectJob {
        VM_INFO,
        BRAND,
        TYPE,
        CONCLUSION,
        IS_VM,
        PERCENTAGE,
        DETECTED_TECHNIQUES,
    };

    class VMDetectWorker : public Napi::AsyncWorker {
    private:
        std::string brand;
        std::string type;
        std::string conclusion;
        bool isVm;
        uint8_t percentage;
        std::vector<std::string> detectedTechniques;

        std::string errorMessage;
        Napi::Promise::Deferred deferred;
        VMOptionsParser parser;

        void doJob(VMDetectJob todo) {
            switch (todo) {
                case VM_INFO: {
                    if (parser.optionExists) {
                        doJob(BRAND);
                        doJob(TYPE);
                        doJob(CONCLUSION);
                        doJob(IS_VM);
                        doJob(PERCENTAGE);
                    } else {
                        VM::vmaware vm;
                        brand = vm.brand;
                        type = vm.type;
                        conclusion = vm.conclusion;
                        isVm = vm.is_vm;
                        percentage = vm.percentage;
                    }
                    doJob(DETECTED_TECHNIQUES);
                    break;
                }
                case BRAND:
                    brand = VM::brand(
                        parser.preset,
                        parser.multiple,
                        parser.highThreshold,
                        parser.dynamic,
                        parser.techniqueFlags
                    );
                    break;
                case TYPE:
                    type = VM::type(
                        parser.preset,
                        parser.multiple,
                        parser.highThreshold,
                        parser.dynamic,
                        parser.techniqueFlags
                    );
                    break;
                case CONCLUSION:
                    conclusion = VM::conclusion(
                        parser.preset,
                        parser.multiple,
                        parser.highThreshold,
                        parser.dynamic,
                        parser.techniqueFlags
                    );
                    break;
                case IS_VM:
                    isVm = VM::detect(
                        parser.preset,
                        parser.multiple,
                        parser.highThreshold,
                        parser.dynamic,
                        parser.techniqueFlags
                    );
                    break;
                case PERCENTAGE:
                    percentage = VM::percentage(
                        parser.preset,
                        parser.multiple,
                        parser.highThreshold,
                        parser.dynamic,
                        parser.techniqueFlags
                    );
                    break;
                case DETECTED_TECHNIQUES: {
                    detectedTechniques.clear();
                    auto detectedFlags = VM::detected_enums(parser.techniqueFlags);
                    for (const auto flag : detectedFlags) {
                        detectedTechniques.push_back(VM::flag_to_string(flag));
                    }
                    break;
                }
            }
        }

        void Resolve(Napi::Value value) noexcept {
            try {
                deferred.Resolve(value);
            } catch (...) {
                // Do nothing if resolving fails, maybe process is already shutting down.
            }
        }

        void Reject(Napi::Value value) noexcept {
            try {
                deferred.Reject(value);
            } catch (...) {
                // Do nothing if rejecting fails, maybe process is already shutting down.
            }
        }

        void Execute() override {
            try {
                doJob(VM_INFO);
            } catch (const std::exception& e) {
                errorMessage = e.what();
            } catch (...) {
                errorMessage = "Unknown error occurred.";
            }
        }

        void OnOK() override {
            if (!NapiActive::IsActive(Env())) {
                return;
            }

            Napi::Env env = Env();
            Napi::HandleScope scope(env);

            if (!errorMessage.empty()) {
                Napi::Error error = Napi::Error::New(env, errorMessage);
                Reject(error.Value());
            } else {
                Napi::Object result = Napi::Object::New(env);

                result.Set("isVM", isVm);
                result.Set("brand", brand);
                result.Set("type", type);
                result.Set("conclusion", conclusion);
                result.Set("percentage", percentage);

                Napi::Array detectedTechniquesArray = Napi::Array::New(env);
                for (size_t i = 0; i < detectedTechniques.size(); ++i) {
                    detectedTechniquesArray.Set(i, detectedTechniques[i]);
                }
                result.Set("detectedTechniques", detectedTechniquesArray);

                Resolve(result);
            }
        }

        void OnError(const Napi::Error& e) override {
            Reject(e.Value());
        }

    public:
        VMDetectWorker(const Napi::CallbackInfo& info)
            : Napi::AsyncWorker(info.Env()),
            deferred(Napi::Promise::Deferred::New(info.Env())),
            parser(info) {}

        Napi::Promise GetPromise() {
            return deferred.Promise();
        }
    };

    static auto GetCallback() {
        return [](const Napi::CallbackInfo& info) {
            VMDetectWorker* worker = new VMDetectWorker(info);
            worker->Queue();
            return worker->GetPromise();
        };
    }

public:
    static void InitVMDetect(Napi::Env env, Napi::Object exports) {
        exports.Set("info", Napi::Function::New(env, GetCallback()));
    }

};
