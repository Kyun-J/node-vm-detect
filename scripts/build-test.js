import { execSync } from 'child_process';
import fs from 'fs';

execSync('npm run bundle');
execSync('npm run gyp');

const test = async () => {
  const { getVMInfo } = await import('../dist/index.esm.js');
  const vmInfo = await getVMInfo();
  console.log('VM Info:', vmInfo);
};

test()
  .catch(console.error)
  .finally(() => {
    fs.rmSync('dist', { recursive: true });
  });
