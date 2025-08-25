import { createRequire } from 'node:module';
import type { SettingFlags, PresetFlags, TechniqueFlags } from './flags';

export interface VMInfo {
  isVM: boolean;
  brand: string;
  type: string;
  conclusion: string;
  percentage: number;
  detectedTechniques: TechniqueFlags[];
}

export interface VMInfoOptions {
  preset?: PresetFlags;
  settings?: SettingFlags[];
  techniques?: {
    only?: TechniqueFlags[];
    disable?: TechniqueFlags[];
  };
}

type NodeVMDetect = {
  info: (options?: VMInfoOptions) => Promise<VMInfo>;
};

export const getNodeVMDetect = Object.assign(
  () => {
    if (getNodeVMDetect.loaded) {
      return getNodeVMDetect.loaded;
    }
    const { platform, arch } = process;
    if (
      !['darwin', 'win32', 'linux'].includes(platform) ||
      !['x64', 'arm64'].includes(arch)
    ) {
      throw new Error('Unsupported platform');
    }
    const loaded: NodeVMDetect = createRequire(import.meta.url)(
      `./${platform}/${arch}/vmDetect.node`,
    );
    getNodeVMDetect.loaded = loaded;
    return loaded;
  },
  {} as { loaded?: NodeVMDetect },
);
