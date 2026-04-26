#!/usr/bin/env node
import { program } from 'commander';
import { isAbsolute, join } from 'node:path';
import { cpSync, existsSync } from 'node:fs';

program
  .command('extract')
  .option('-d, --destination <destination>', 'destination path')
  .option('-p, --platform <platforms...>', 'darwin, linux, win32')
  .option('-a, --arch <archs...>', 'x64, arm64')
  .option('-c, --child', 'extract child process file')
  .action(
    (options?: {
      platform?: string[];
      arch?: string[];
      destination?: string;
      child?: boolean;
    }) => {
      const destination = options?.destination
        ? isAbsolute(options.destination)
          ? options.destination
          : join(process.cwd(), options.destination)
        : join(process.cwd(), 'node-vm-detect');
      const platforms = options?.platform ?? ['darwin', 'linux', 'win32'];
      const archs = options?.arch ?? ['x64', 'arm64'];
      platforms.forEach((platform) => {
        archs.forEach((arch) => {
          const sourcePath = join(__dirname, platform, arch, 'vmDetect.node');
          if (!existsSync(sourcePath)) {
            console.error(`Unsupported platform: ${platform} ${arch}`);
            return;
          }
          const destinationPath = join(
            destination,
            platform,
            arch,
            'vmDetect.node',
          );
          cpSync(sourcePath, destinationPath, { recursive: true });
        });
      });
      if (options?.child) {
        const childModulePath = join(__dirname, 'node-vm-detect-child.js');
        cpSync(childModulePath, join(destination, 'node-vm-detect-child.js'), {
          recursive: true,
        });
      }
    },
  );

program.parse();
