#!/usr/local/bin/wsbasic

begin

  ###################################
  #foreach met default seperator = \n
  #
  i=0
  foreach line in run("ls -al")
  begin
    i=i+1
    println i,".\t", line
  end


  print "\n\nnow seperate on '.b'\n"

  #################################
  #foreach with different seperator
  #
  i=0
  foreach line in run("ls -al") seperated by ".b\n"
  begin
    i=i+1
    println i,".\t", line
  end

end

