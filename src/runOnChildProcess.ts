import { fork } from 'node:child_process';
import type { VMInfo, VMInfoOptions } from './nodeVMDetect';
import type { ParentEvent, WorkerEvent } from './worker';
import { join, dirname } from 'node:path';
import { fileURLToPath } from 'node:url';

export const runOnChildProcess = (options?: VMInfoOptions) => {
  return new Promise<VMInfo>((resolve, reject) => {
    const child = fork(
      join(dirname(fileURLToPath(import.meta.url)), 'worker.js'),
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
  });
};
