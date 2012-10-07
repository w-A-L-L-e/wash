#!/usr/local/bin/wsbasic
begin

  #string operations
  a="hello"
  b="world"
  c=a+" "+b
  println c


  #mixing types  
  i=5
  test="hello " + i
  println test


  #let's see if i still works
  println i*5

  a="abcdefgh"
  println "a=",a," -> substr=",substr(a,1,4)
    
end
