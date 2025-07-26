import { execSync } from 'child_process';

execSync('npm run bundle');

if (process.platform === 'win32') {
  if (process.arch === 'x64') {
    execSync('npm run gyp:win:x64');
  }
  if (process.arch === 'arm64') {
    execSync('npm run gyp:win:arm64');
  }
} else if (process.platform === 'darwin') {
  execSync('npm run gyp:mac');
}

const test = async () => {
  const { getVMInfo } = await import('../dist/index.esm.js');
  const vmInfo = await getVMInfo();
  console.log('VM Info:', vmInfo);
};

test();
