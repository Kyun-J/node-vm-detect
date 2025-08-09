#pragma once

#include <napi.h>

struct NapiActive {
private:
    struct ActiveData {
        std::atomic<bool> active = true;
    };

public:
    static void InitNapiActive(Napi::Env env) {
        auto* data = new ActiveData();
        env.SetInstanceData(data);
        env.AddCleanupHook([](void* data) {
            static_cast<ActiveData*>(data)->active.store(false, std::memory_order_relaxed);
        }, data);
    }

    static bool IsActive(Napi::Env env) {
        auto* data = env.GetInstanceData<ActiveData>();
        return data->active.load(std::memory_order_acquire);
    }
};
