import { getNodeVMDetect } from './nodeVMDetect';
import type { VMInfoOptions, VMInfo } from './nodeVMDetect';
import { runOnChildProcess } from './runOnChildProcess';

export interface GetVMInfoOptions extends VMInfoOptions {
  runOnChild?: boolean | string;
  moduleRoot?: string;
}

export const getVMInfo = (options?: GetVMInfoOptions): Promise<VMInfo> => {
  if (options?.runOnChild) {
    return runOnChildProcess(options);
  } else {
    return getNodeVMDetect(options?.moduleRoot).info(options);
  }
};

export type { VMInfo };
export type * from './flags';
