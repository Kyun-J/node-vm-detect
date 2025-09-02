import { getNodeVMDetect } from './nodeVMDetect';
import type { VMInfoOptions, VMInfo } from './nodeVMDetect';
import { runOnChildProcess } from './runOnChildProcess';

export interface GetVMInfoOptions extends VMInfoOptions {
  clean?: boolean;
}

export const getVMInfo = (options?: GetVMInfoOptions): Promise<VMInfo> => {
  if (options?.clean === false) {
    return getNodeVMDetect().info(options);
  } else {
    return runOnChildProcess(options);
  }
};

export type { VMInfo };
export type * from './flags';
