#!/usr/local/bin/wsbasic
begin

  #test mixing ops
  a=2+3-4+1  
  println a

  #test braces
  b=4*(2+3)
  println "b=",b
  
  #test operator precedence
  c=4*2+3
  println "c=",c
  
  #test unary minus
  tricky=-2.2+3.1
  println "tricky=",tricky

  #boolean expressions
  b1= (2>3) or (2<3)
  println "b1=",b1
  
  b2= (2>3) and (2<3)
  println "b2=",b2

  b3= not ( (2>3) and (2<3) )
  println "b3=",b3

  f = 3
  g = -f *2
  println "g=",g  
  
end
