import { fork } from 'node:child_process';
import type { VMInfo } from './nodeVMDetect';
import type { ParentEvent, WorkerEvent } from './worker';
import { join, dirname } from 'node:path';
import { fileURLToPath } from 'node:url';
import { GetVMInfoOptions } from '.';

export const runOnChildProcess = (options?: GetVMInfoOptions) => {
  return new Promise<VMInfo>((resolve, reject) => {
    try {
      const childModulePath =
        typeof options?.runOnChild === 'string' && options.runOnChild;
      const child = fork(
        childModulePath ||
          join(
            dirname(fileURLToPath(import.meta.url)),
            'node-vm-detect-child.js',
          ),
        (process as { electron?: string }).electron
          ? {
              execPath: process.execPath,
              env: { ...process.env, ELECTRON_RUN_AS_NODE: '1' },
            }
          : {},
      );
      const sendMessage = (message: ParentEvent) => {
        child.send(message);
      };
      const killChild = () => {
        sendMessage({ event: 'kill' });
        child.kill();
      };
      child.on('message', ({ event }: { event: WorkerEvent }) => {
        switch (event.event) {
          case 'vmInfo':
            killChild();
            resolve(event.data);
            break;
          case 'error':
            killChild();
            reject(event.error);
            break;
        }
      });
      sendMessage({ event: 'vmInfo', data: options });
    } catch (error) {
      reject(error);
    }
  });
};
