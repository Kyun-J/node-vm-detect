#include <napi.h>
#include "vmaware.hpp"

struct VMDetectWorkerData {
    napi_async_work work;
    napi_deferred deferred;
    VM::vmaware vm;
    std::string errorMessage;
};

void VMDetectExecuteWorker(napi_env env, void* data) {
    VMDetectWorkerData* workerData = (VMDetectWorkerData*)data;
    try {
        workerData->vm = VM::vmaware();
    } catch (const std::exception& e) {
        workerData->errorMessage = e.what();
    } catch (...) {
        workerData->errorMessage = "Unknown error occurred.";
    }
}

void VMDetectCompleteWorker(napi_env env, napi_status status, void* data) {
    VMDetectWorkerData* workerData = (VMDetectWorkerData*)data;

    if (status != napi_ok) {
        napi_value error;
        napi_create_string_utf8(env, "Error occurred during asynchronous operation", NAPI_AUTO_LENGTH, &error);
        napi_reject_deferred(env, workerData->deferred, error);
    } else if (!workerData->errorMessage.empty()) {
        napi_value error;
        napi_create_string_utf8(env, workerData->errorMessage.c_str(), NAPI_AUTO_LENGTH, &error);
        napi_reject_deferred(env, workerData->deferred, error);
    } else {
        napi_value result;
        napi_create_object(env, &result);

        napi_value isVM;
        napi_get_boolean(env, workerData->vm.is_vm, &isVM);
        napi_set_named_property(env, result, "isVM", isVM);

        napi_value brand;
        napi_create_string_utf8(env, workerData->vm.brand.c_str(), NAPI_AUTO_LENGTH, &brand);
        napi_set_named_property(env, result, "brand", brand);

        napi_value type;
        napi_create_string_utf8(env, workerData->vm.type.c_str(), NAPI_AUTO_LENGTH, &type);
        napi_set_named_property(env, result, "type", type);

        napi_value conclusion;
        napi_create_string_utf8(env, workerData->vm.conclusion.c_str(), NAPI_AUTO_LENGTH, &conclusion);
        napi_set_named_property(env, result, "conclusion", conclusion);

        napi_value percentage;
        napi_create_int32(env, workerData->vm.percentage, &percentage);
        napi_set_named_property(env, result, "percentage", percentage);

        napi_value detected_count;
        napi_create_int32(env, workerData->vm.detected_count, &detected_count);
        napi_set_named_property(env, result, "detectedCount", detected_count);

        napi_value technique_count;
        napi_create_int32(env, workerData->vm.technique_count, &technique_count);
        napi_set_named_property(env, result, "techniqueCount", technique_count);

        napi_resolve_deferred(env, workerData->deferred, result);
    }

    napi_delete_async_work(env, workerData->work);
    delete workerData;
}

napi_value GetVMInfo(napi_env env, napi_callback_info info) {
    napi_value promise;
    napi_deferred deferred;
    napi_create_promise(env, &deferred, &promise);

    VMDetectWorkerData* workerData = new VMDetectWorkerData();
    workerData->deferred = deferred;

    napi_value resourceName;
    napi_create_string_utf8(env, "GetVMInfo", NAPI_AUTO_LENGTH, &resourceName);

    napi_create_async_work(env, NULL, resourceName, VMDetectExecuteWorker, VMDetectCompleteWorker, workerData, &workerData->work);
    napi_queue_async_work(env, workerData->work);

    return promise;
}

void InitGetVMInfo(napi_env env, napi_value exports) {
    napi_property_descriptor desc[] = {
        { "getVMInfo", 0, GetVMInfo, 0, 0, 0, napi_default, 0 }
    };
    napi_define_properties(env, exports, 1, desc);
}
