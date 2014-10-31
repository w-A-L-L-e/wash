#!/bin/wash
begin

  #simple for loops
  println "simple for:"
  for i=0 to 5
    println "i=",i

  println "i after loop=",i


  
  
  #nested for loop
  println "now a nested for:"
    
  for i=1 to 3
    for j=1 to 3 begin
      println "i=",i," j=",j
    end


  println "for loop with step:"
  #for loop with a step
  for i=10 to 1 step -2
  begin
    println "i=",i
  end
  
  println "for loop with wrong step"
  a=-5
  b=1
  for i=5 to a step b
    println "i=",i

end

