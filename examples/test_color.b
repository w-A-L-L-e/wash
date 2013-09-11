#!/usr/local/bin/wsbasic

begin
  result=run("export CLICOLOR_FORCE=Yes && export CLICOLOR=Yes && export TERM=xterm-256color && ls -C")
  print( result )
  
end

