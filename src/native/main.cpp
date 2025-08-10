#include <napi.h>
#include "napiActive.hpp"
#include "vmDetect.hpp"


Napi::Object Init(Napi::Env env, Napi::Object exports) {
    NapiActive::InitNapiActive(env);
    VMDetect::InitGetVMDetect(env, exports);
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
