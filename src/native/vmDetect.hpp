#include <napi.h>
#include <map>
#include <string>
#include "vmaware.hpp"
#include "napiActive.hpp"

struct VMDetect {
private:

    struct VMDetector {
    private:
        using vmFlagset = std::bitset<VM::MULTIPLE + 1>;

        std::map<std::string, std::pair<VM::enum_flags, VM::enum_flags>> presetFlagMap =
        {
            {"ALL", {VM::ALL, VM::NULL_ARG}},
            {"DEFAULT", {VM::DEFAULT, VM::NULL_ARG}},
        };

        std::map<std::string, std::pair<VM::enum_flags, VM::enum_flags>> settingFlagMap =
        {
            {"MULTIPLE", {VM::MULTIPLE, VM::NULL_ARG}},
            {"HIGH_THRESHOLD", {VM::HIGH_THRESHOLD, VM::NULL_ARG}},
            {"DYNAMIC", {VM::DYNAMIC, VM::NULL_ARG}},
        };

        std::map<std::string, std::pair<VM::enum_flags, VM::enum_flags>> techniqueFlagMap =
        {
            // Windows
            {"GPU_CAPABILITIES", {VM::GPU_CAPABILITIES, VM::GPU_CAPABILITIES}},
            {"ACPI_SIGNATURE", {VM::ACPI_SIGNATURE, VM::ACPI_SIGNATURE}},
            {"POWER_CAPABILITIES", {VM::POWER_CAPABILITIES, VM::POWER_CAPABILITIES}},
            {"DISK_SERIAL", {VM::DISK_SERIAL, VM::DISK_SERIAL}},
            {"IVSHMEM", {VM::IVSHMEM, VM::IVSHMEM}},
            {"DRIVERS", {VM::DRIVERS, VM::DRIVERS}},
            {"DEVICE_HANDLES", {VM::DEVICE_HANDLES, VM::DEVICE_HANDLES}},
            {"VIRTUAL_PROCESSORS", {VM::VIRTUAL_PROCESSORS, VM::VIRTUAL_PROCESSORS}},
            {"HYPERVISOR_QUERY", {VM::HYPERVISOR_QUERY, VM::HYPERVISOR_QUERY}},
            {"AUDIO", {VM::AUDIO, VM::AUDIO}},
            {"DISPLAY", {VM::DISPLAY, VM::DISPLAY}},
            {"DLL", {VM::DLL, VM::DLL}},
            {"VMWARE_BACKDOOR", {VM::VMWARE_BACKDOOR, VM::VMWARE_BACKDOOR}},
            {"WINE", {VM::WINE, VM::WINE}},
            {"VIRTUAL_REGISTRY", {VM::VIRTUAL_REGISTRY, VM::VIRTUAL_REGISTRY}},
            {"MUTEX", {VM::MUTEX, VM::MUTEX}},
            {"DEVICE_STRING", {VM::DEVICE_STRING, VM::DEVICE_STRING}},
            {"VPC_INVALID", {VM::VPC_INVALID, VM::VPC_INVALID}},
            {"VMWARE_STR", {VM::VMWARE_STR, VM::VMWARE_STR}},
            {"GAMARUE", {VM::GAMARUE, VM::GAMARUE}},
            {"CUCKOO_DIR", {VM::CUCKOO_DIR, VM::CUCKOO_DIR}},
            {"CUCKOO_PIPE", {VM::CUCKOO_PIPE, VM::CUCKOO_PIPE}},
            {"BOOT_LOGO", {VM::BOOT_LOGO, VM::BOOT_LOGO}},
            {"TRAP", {VM::TRAP, VM::TRAP}},
            {"UD", {VM::UD, VM::UD}},
            {"BLOCKSTEP", {VM::BLOCKSTEP, VM::BLOCKSTEP}},
            {"DBVM", {VM::DBVM, VM::DBVM}},
            {"KERNEL_OBJECTS", {VM::KERNEL_OBJECTS, VM::KERNEL_OBJECTS}},
            {"NVRAM", {VM::NVRAM, VM::NVRAM}},
            {"SMBIOS_INTEGRITY", {VM::SMBIOS_INTEGRITY, VM::SMBIOS_INTEGRITY}},
            {"EDID", {VM::EDID, VM::EDID}},
            {"CPU_HEURISTIC", {VM::CPU_HEURISTIC, VM::CPU_HEURISTIC}},
            {"CLOCK", {VM::CLOCK, VM::CLOCK}},

            // Linux and Windows
            {"SYSTEM_REGISTERS", {VM::SYSTEM_REGISTERS, VM::SYSTEM_REGISTERS}},
            {"FIRMWARE", {VM::FIRMWARE, VM::FIRMWARE}},
            {"PCI_DEVICES", {VM::PCI_DEVICES, VM::PCI_DEVICES}},
            {"AZURE", {VM::AZURE, VM::AZURE}},

            // Linux
            {"SMBIOS_VM_BIT", {VM::SMBIOS_VM_BIT, VM::SMBIOS_VM_BIT}},
            {"KMSG", {VM::KMSG, VM::KMSG}},
            {"CVENDOR", {VM::CVENDOR, VM::CVENDOR}},
            {"QEMU_FW_CFG", {VM::QEMU_FW_CFG, VM::QEMU_FW_CFG}},
            {"SYSTEMD", {VM::SYSTEMD, VM::SYSTEMD}},
            {"CTYPE", {VM::CTYPE, VM::CTYPE}},
            {"DOCKERENV", {VM::DOCKERENV, VM::DOCKERENV}},
            {"DMIDECODE", {VM::DMIDECODE, VM::DMIDECODE}},
            {"DMESG", {VM::DMESG, VM::DMESG}},
            {"HWMON", {VM::HWMON, VM::HWMON}},
            {"LINUX_USER_HOST", {VM::LINUX_USER_HOST, VM::LINUX_USER_HOST}},
            {"VMWARE_IOMEM", {VM::VMWARE_IOMEM, VM::VMWARE_IOMEM}},
            {"VMWARE_IOPORTS", {VM::VMWARE_IOPORTS, VM::VMWARE_IOPORTS}},
            {"VMWARE_SCSI", {VM::VMWARE_SCSI, VM::VMWARE_SCSI}},
            {"VMWARE_DMESG", {VM::VMWARE_DMESG, VM::NULL_ARG}},
            {"QEMU_VIRTUAL_DMI", {VM::QEMU_VIRTUAL_DMI, VM::QEMU_VIRTUAL_DMI}},
            {"QEMU_USB", {VM::QEMU_USB, VM::QEMU_USB}},
            {"HYPERVISOR_DIR", {VM::HYPERVISOR_DIR, VM::HYPERVISOR_DIR}},
            {"UML_CPU", {VM::UML_CPU, VM::UML_CPU}},
            {"VBOX_MODULE", {VM::VBOX_MODULE, VM::VBOX_MODULE}},
            {"SYSINFO_PROC", {VM::SYSINFO_PROC, VM::SYSINFO_PROC}},
            {"DMI_SCAN", {VM::DMI_SCAN, VM::DMI_SCAN}},
            {"PODMAN_FILE", {VM::PODMAN_FILE, VM::PODMAN_FILE}},
            {"WSL_PROC", {VM::WSL_PROC, VM::WSL_PROC}},
            {"FILE_ACCESS_HISTORY", {VM::FILE_ACCESS_HISTORY, VM::FILE_ACCESS_HISTORY}},
            {"MAC", {VM::MAC, VM::MAC}},
            {"NSJAIL_PID", {VM::NSJAIL_PID, VM::NSJAIL_PID}},
            {"BLUESTACKS_FOLDERS", {VM::BLUESTACKS_FOLDERS, VM::BLUESTACKS_FOLDERS}},
            {"AMD_SEV", {VM::AMD_SEV, VM::AMD_SEV}},
            {"TEMPERATURE", {VM::TEMPERATURE, VM::TEMPERATURE}},
            {"PROCESSES", {VM::PROCESSES, VM::PROCESSES}},

            // Linux and MacOS
            {"THREAD_COUNT", {VM::THREAD_COUNT, VM::THREAD_COUNT}},

            // MacOS
            {"MAC_MEMSIZE", {VM::MAC_MEMSIZE, VM::MAC_MEMSIZE}},
            {"MAC_IOKIT", {VM::MAC_IOKIT, VM::MAC_IOKIT}},
            {"MAC_SIP", {VM::MAC_SIP, VM::MAC_SIP}},
            {"IOREG_GREP", {VM::IOREG_GREP, VM::IOREG_GREP}},
            {"HWMODEL", {VM::HWMODEL, VM::HWMODEL}},
            {"MAC_SYS", {VM::MAC_SYS, VM::MAC_SYS}},

            // cross-platform
            {"HYPERVISOR_BIT", {VM::HYPERVISOR_BIT, VM::HYPERVISOR_BIT}},
            {"VMID", {VM::VMID, VM::VMID}},
            {"THREAD_MISMATCH", {VM::THREAD_MISMATCH, VM::THREAD_MISMATCH}},
            {"TIMER", {VM::TIMER, VM::TIMER}},
            {"CPU_BRAND", {VM::CPU_BRAND, VM::CPU_BRAND}},
            {"HYPERVISOR_STR", {VM::HYPERVISOR_STR, VM::HYPERVISOR_STR}},
            {"CPUID_SIGNATURE", {VM::CPUID_SIGNATURE, VM::CPUID_SIGNATURE}},
            {"BOCHS_CPU", {VM::BOCHS_CPU, VM::BOCHS_CPU}},
            {"KGT_SIGNATURE", {VM::KGT_SIGNATURE, VM::KGT_SIGNATURE}},
        };

        std::array<VM::enum_flags, VM::MULTIPLE + 1> args;

        void generateArgs() {
            std::memset(args.data(), VM::NULL_ARG, sizeof(args));
            uint8_t index = 0;
            for (auto& [_, value] : presetFlagMap) {
                args[index] = value.second;
                index++;
            }
            for (auto& [_, value] : settingFlagMap) {
                args[index] = value.second;
                index++;
            }
            for (auto& [_, value] : techniqueFlagMap) {
                args[index] = value.second;
                index++;
            }
        }

    public:
        struct VMInfo {
            std::string brand;
            std::string type;
            std::string conclusion;
            bool isVm;
            uint8_t percentage;
            std::vector<std::string> detectedTechniques;

            VMInfo(VMDetector& detector) {
                detector.generateArgs();
                brand = std::apply([](auto... args) { return VM::brand(args...); }, detector.args);
                type = std::apply([](auto... args) { return VM::type(args...); }, detector.args);
                conclusion = std::apply([](auto... args) { return VM::conclusion(args...); }, detector.args);
                isVm = std::apply([](auto... args) { return VM::detect(args...); }, detector.args);
                percentage = std::apply([](auto... args) { return VM::percentage(args...); }, detector.args);
                auto detectedFlags = std::apply([](auto... args) { return VM::detected_enums(args...); }, detector.args);
                detectedTechniques.reserve(detectedFlags.size());
                for (const auto flag : detectedFlags) {
                    detectedTechniques.push_back(VM::flag_to_string(flag));
                }
            }
        };

        VMDetector(const Napi::CallbackInfo& info) {
            Napi::Object options;
            if (info.Length() > 0 && info[0].IsObject()) {
                options = info[0].As<Napi::Object>();
            } else {
                return;
            }

            auto settings = options.Get("settings");
            if (settings.IsArray()) {
                auto settingsArray = settings.As<Napi::Array>();
                uint8_t length = settingsArray.Length();
                for (uint8_t i = 0; i < length; i++) {
                    Napi::Value flagValue = settingsArray[i];
                    if (!flagValue.IsString()) {
                        continue;
                    }
                    auto flagStr = flagValue.As<Napi::String>();
                    auto it = settingFlagMap.find(flagStr);
                    if (it != settingFlagMap.end()) {
                        it->second.second = it->second.first;
                    }
                }
            }

            auto techniques = options.Get("techniques");
            if (techniques.IsString()) {
                auto presetStr = techniques.As<Napi::String>();
                auto it = presetFlagMap.find(presetStr);
                if (it != presetFlagMap.end()) {
                    it->second.second = it->second.first;
                }
            } else if (techniques.IsObject()) {
                auto techniquesObject = techniques.As<Napi::Object>();

                auto only = techniquesObject.Get("only");
                if (only.IsArray()) {
                    auto onlyArray = only.As<Napi::Array>();
                    uint8_t length = onlyArray.Length();
                    bool reset = false;
                    for (uint8_t i = 0; i < length; i++) {
                        Napi::Value flagValue = onlyArray[i];
                        if (flagValue.IsString()) {
                            auto flagStr = flagValue.As<Napi::String>();
                            auto it = techniqueFlagMap.find(flagStr);
                            if (it != techniqueFlagMap.end()) {
                                if (!reset) {
                                    for (auto& [key, value] : techniqueFlagMap) {
                                        value.second = VM::NULL_ARG;
                                    }
                                    reset = true;
                                }
                                it->second.second = it->second.first;
                            }
                        }
                    }
                }

                auto disable = techniquesObject.Get("disable");
                if (disable.IsArray()) {
                    auto disableArray = disable.As<Napi::Array>();
                    uint8_t length = disableArray.Length();
                    for (uint8_t i = 0; i < length; i++) {
                        Napi::Value flagValue = disableArray[i];
                        if (flagValue.IsString()) {
                            auto flagStr = flagValue.As<Napi::String>();
                            auto it = techniqueFlagMap.find(flagStr);
                            if (it != techniqueFlagMap.end()) {
                                it->second.second = VM::NULL_ARG;
                            }
                        }
                    }
                }
            }
        }

        auto generateInfo() {
            return VMInfo(*this);
        }
    };

    class VMDetectWorker : public Napi::AsyncWorker {
    private:
        std::string errorMessage;
        Napi::Promise::Deferred deferred;
        VMDetector detector;
        std::optional<VMDetector::VMInfo> info;

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
                info = detector.generateInfo();
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
            } else if (!info.has_value()) {
                Napi::Error error = Napi::Error::New(env, "No info available");
                Reject(error.Value());
            } else {
                Napi::Object result = Napi::Object::New(env);

                result.Set("isVM", info->isVm);
                result.Set("brand", info->brand);
                result.Set("type", info->type);
                result.Set("conclusion", info->conclusion);
                result.Set("percentage", info->percentage);

                Napi::Array detectedTechniquesArray = Napi::Array::New(env);
                uint8_t length = info->detectedTechniques.size();
                for (uint8_t i = 0; i < length; i++) {
                    detectedTechniquesArray.Set(i, info->detectedTechniques[i]);
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
            detector(info) {}

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
