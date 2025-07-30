import { execSync } from 'child_process';
import fs from 'fs';

fs.mkdirSync('./dist/mac/x64', { recursive: true });
execSync('arch -x86_64 node-gyp rebuild --arch=x64');
fs.copyFileSync(
  './build/Release/vmDetect.node',
  './dist/mac/x64/vmDetect.node',
);

if (process.arch === 'arm64') {
  fs.mkdirSync('./dist/mac/arm64', { recursive: true });
  execSync('node-gyp rebuild --arch=arm64');
  fs.copyFileSync(
    './build/Release/vmDetect.node',
    './dist/mac/arm64/vmDetect.node',
  );
}
