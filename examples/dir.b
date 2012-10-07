#!/usr/local/bin/wsbasic

begin

  for i=2 to run("ls -al | wc -l")
  begin
    line=run("ls -al | head -n " + i + " | tail -n 1")
    print i-1,".\t", line
  end

end

