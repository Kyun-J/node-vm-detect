#include <napi.h>

void InitGetVMInfo(napi_env env, napi_value exports);

napi_value Init(napi_env env, napi_value exports) {
    InitGetVMInfo(env, exports);
    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
