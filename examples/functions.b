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

#function with parameters and brackets c-style!
test2(a,b,c,d){
  println "a=", a, ", b=", b, ", c=", c
  println "a+b+c= ", a+b+c
  println ""
}


#function that returns something
square(i){
  return i*i
}


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
  test2(1,2,3,4)

  j=1
  while j<10 begin
    println "fac(",j,")=",fac(j)
    j=j+1
  end
  
  println ""
  println "glob        =" , glob
  println "square(glob)=" , square(glob) 
end

