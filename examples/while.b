#!/usr/local/bin/wsbasic
begin
  ####################
  #first type of while
  #
  i=0
  while i+1<5*200 i=i+1 
  println "i after first while = ",i 

  #################
  #while with block
  #
  i=1
  while i<10*2 
  begin
    i=i+1
    print "i=", i, " "
    println "i+1=",i+1
  end
end
