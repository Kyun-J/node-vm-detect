import { createRequire } from 'node:module';
import type { PresetFlags, SettingFlags, TechniqueFlags } from './flags';

export interface VMInfo {
  isVM: boolean;
  brand: string;
  type: string;
  conclusion: string;
  percentage: number;
  detectedCount: number;
}

export interface VMInfoOptions {
  preset?: PresetFlags;
  settings?: SettingFlags[];
  techniques?: {
    only?: TechniqueFlags[];
    disable?: TechniqueFlags[];
  };
}

const nodeVMDetect: {
  info: (options?: VMInfoOptions) => Promise<VMInfo>;
  isVM: (options?: VMInfoOptions) => Promise<boolean>;
  brand: (options?: VMInfoOptions) => Promise<string>;
  type: (options?: VMInfoOptions) => Promise<string>;
  conclusion: (options?: VMInfoOptions) => Promise<string>;
  percentage: (options?: VMInfoOptions) => Promise<number>;
  detectedCount: (options?: VMInfoOptions) => Promise<number>;
} = (() => {
  const { platform, arch } = process;
  if (
    !['darwin', 'win32', 'linux'].includes(platform) ||
    !['x64', 'arm64'].includes(arch)
  ) {
    throw new Error('Unsupported platform');
  }
  return createRequire(import.meta.url)(`./${platform}/${arch}/vmDetect.node`);
})();

export default nodeVMDetect;

export const getVMInfo = (options?: VMInfoOptions): Promise<VMInfo> =>
  nodeVMDetect.info(options);

export const getIsVM = (options?: VMInfoOptions): Promise<boolean> =>
  nodeVMDetect.isVM(options);

export const getBrand = (options?: VMInfoOptions): Promise<string> =>
  nodeVMDetect.brand(options);

export const getType = (options?: VMInfoOptions): Promise<string> =>
  nodeVMDetect.type(options);

export const getConclusion = (options?: VMInfoOptions): Promise<string> =>
  nodeVMDetect.conclusion(options);

export const getPercentage = (options?: VMInfoOptions): Promise<number> =>
  nodeVMDetect.percentage(options);

export const getDetectedCount = (options?: VMInfoOptions): Promise<number> =>
  nodeVMDetect.detectedCount(options);
