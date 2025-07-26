import { execSync } from 'child_process';

execSync('rmdir /s /q .\\dist\\win');

execSync('mkdir .\\dist\\win\\x64');
execSync('node-gyp rebuild --arch=x64');
execSync(
  'robocopy build\\Release dist\\win\\x64 detectWindows.node detectWindows.pdb /move',
);

execSync('mkdir .\\dist\\win\\arm64');
execSync('node-gyp rebuild --arch=arm64');
execSync(
  'robocopy build\\Release dist\\win\\arm64 detectWindows.node detectWindows.pdb /move',
);
