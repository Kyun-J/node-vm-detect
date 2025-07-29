import { createRequire } from 'node:module';
import type { CoreSettingFlags, TechniqueFlags } from './flags';

export interface VMInfo {
  isVM: boolean;
  brand: string;
  type: string;
  conclusion: string;
  percentage: number;
  detectedCount: number;
}

export type GetVMInfoOptions = {
  core?: CoreSettingFlags;
  disable?: TechniqueFlags[];
};

let napi: {
  getVMInfo: (options?: GetVMInfoOptions) => Promise<VMInfo>;
};

const getNapi = () => {
  if (napi) {
    return napi;
  }

  let napiPath: string | undefined;
  if (process.platform === 'win32') {
    if (process.arch === 'x64') {
      napiPath = './win/x64/detectWindows.node';
    }
  } else if (process.platform === 'darwin') {
    if (process.arch === 'arm64') {
      napiPath = './mac/arm64/detectMac.node';
    } else if (process.arch === 'x64') {
      napiPath = './mac/x64/detectMac.node';
    }
  } else if (process.platform === 'linux') {
    if (process.arch === 'x64') {
      napiPath = './linux/x64/detectLinux.node';
    } else if (process.arch === 'arm64') {
      napiPath = './linux/arm64/detectLinux.node';
    }
  }
  if (!napiPath) {
    throw new Error('Unsupported platform');
  }

  napi = createRequire(import.meta.url)(napiPath);
  return napi;
};

export const getVMInfo = async (
  options?: GetVMInfoOptions,
): Promise<VMInfo> => {
  return getNapi().getVMInfo(options);
};
