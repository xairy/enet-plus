#!/usr/bin/python

import fnmatch
import sys
import subprocess

def git_get_staged_files():
  files = str(subprocess.check_output(['git', 'diff', '--name-only', '--cached']))
  return [line for line in files.splitlines() if line != '']

def git_get_all_files():
  files = str(subprocess.check_output(['git', 'ls-files']))
  return [line for line in files.splitlines() if line != '']

def get_src_files():
  files = git_get_all_files()
  extensions = ('c', 'cpp', 'h', 'hpp')
  return [file for file in files if (file.startswith('src/') or file.startswith('inc')) and file.endswith(extensions)]

def get_staged_src_files():
  files = git_get_staged_files()
  extensions = ('c', 'cpp', 'h', 'hpp')
  return [file for file in files if file.startswith('src/') and file.endswith(extensions)]

def get_blacklisted_files():
  with open('blacklist', 'r') as blacklist:
    return [line.rstrip() for line in blacklist if line != '' and line.lstrip()[0] != '#']

def cpplint_check_file(filename):
  try:
    filters = '--filter=-readability/check,-build/include_order,-build/header_guard'
    subprocess.check_call(['python', 'cpplint.py', '--root=src', filters, filename])
  except subprocess.CalledProcessError as error:
    return False
  return True

def main():
  src_files = get_staged_src_files()
  if len(sys.argv) == 2 and sys.argv[1] == 'all':
    src_files = get_src_files()
  blacklisted_files = get_blacklisted_files()
  files_to_check = [file for file in src_files if file not in blacklisted_files]

  bad_files_count = 0
  for file in files_to_check:
    if not cpplint_check_file(file):
      bad_files_count += 1

  sys.exit(bad_files_count)

if __name__ == '__main__':
  main()
