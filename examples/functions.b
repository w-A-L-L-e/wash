#!/usr/local/bin/wsbasic


#function without parameters
hello()
begin
  println "hello there!"
end

#function with parameters
test(a,b,c)
begin
  println "a=", a, ", b=", b, ", c=", c
  println "a+b+c= ", a+b+c
  println ""
end


#function that returns something
square(i)
begin
  return i*i
end


fac(i)
begin
  if(i<=1) begin
    return 1
  end
  else begin
    return i*fac(i-1)
  end
end

begin
  glob=5
  hello()
  test(1,2,3)
  
  j=1
  while j<10 begin
    println "fac(",j,")=",fac(j)
    j=j+1
  end
  
  println ""
  println "glob        =" , glob
  println "square(glob)=" , square(glob) 
end

