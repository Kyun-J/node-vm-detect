import prettierRecommended from 'eslint-plugin-prettier/recommended';
import eslint from '@eslint/js';
import tseslint from 'typescript-eslint';

export default tseslint.config({
  extends: [
    eslint.configs.recommended,
    tseslint.configs.recommended,
    prettierRecommended,
  ],
  ignores: ['dist/**/*', 'build/**/*', 'VMAware/**/*'],
  files: ['**/*.ts', '**/*.js'],
  rules: {
    'prettier/prettier': [
      'error',
      {
        endOfLine: 'auto',
      },
    ],
  },
});
