import { getNodeVMDetect } from './nodeVMDetect';
import type { VMInfoOptions, VMInfo } from './nodeVMDetect';
import { runOnChildProcess } from './runOnChildProcess';

export type { VMInfoOptions, VMInfo };
export type * from './flags';

export const getVMInfo = (
  options?: VMInfoOptions,
  clean?: boolean,
): Promise<VMInfo> => {
  if (clean) {
    return runOnChildProcess(options);
  } else {
    return getNodeVMDetect().info(options);
  }
};
