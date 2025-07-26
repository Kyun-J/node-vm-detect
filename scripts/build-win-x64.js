import { execSync } from 'child_process';
import fs from 'fs';

fs.mkdirSync('./dist/win/x64', { recursive: true });
execSync('node-gyp rebuild --arch=x64');
fs.copyFileSync(
  './build/Release/detectWindows.node',
  './dist/win/x64/detectWindows.node',
);
fs.copyFileSync(
  './build/Release/detectWindows.pdb',
  './dist/win/x64/detectWindows.pdb',
);
