import { getNodeVMDetect } from './nodeVMDetect';
import type { VMInfoOptions, VMInfo } from './nodeVMDetect';

type VMInfoResponseEvent = {
  event: 'vmInfo';
  data: VMInfo;
};

type ErrorEvent = {
  event: 'error';
  error: unknown;
};

export type WorkerEvent = VMInfoResponseEvent | ErrorEvent;

type VMInfoRequestEvent = {
  event: 'vmInfo';
  data?: VMInfoOptions;
};

type KillEvent = {
  event: 'kill';
};

export type ParentEvent = VMInfoRequestEvent | KillEvent;

const sendEvent = (event: WorkerEvent) => {
  process.send?.({ event });
};

process.on('message', (event: ParentEvent) => {
  switch (event.event) {
    case 'vmInfo':
      try {
        getNodeVMDetect()
          .info(event.data)
          .then((data) => sendEvent({ event: 'vmInfo', data }))
          .catch((error) => sendEvent({ event: 'error', error }));
      } catch (error) {
        sendEvent({ event: 'error', error });
      }
      break;
    case 'kill':
      process.exit(0);
  }
});

process.on('SIGTERM', () => process.exit(0));
process.on('SIGINT', () => process.exit(0));
