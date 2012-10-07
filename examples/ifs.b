#!/usr/local/bin/wsbasic
begin

  ##########################
  #simple if's without else
  #
  a=2
  if (2<3)
    b=1
  
  if a<3 begin
    b=2
    b=b/2
  end
  


  ####################
  #if's with an else
  #
  if a>2 
    b=0
  else
    b=5
    


  if a>2 begin
    b=6
    b=b*2
  end
  else begin
    b=2
    b=b*3
  end

  c=3
  
  if c==1
    print "nee "
  else if c==3
    println "jaja!"
  else
    println "hier moeten we nie zijn"
    

  
  println b

  a=1
  if a==1 println "a is 1"
  if a>0  println "a>0"
  if 0<a  println "0<a"
  if a>=1 println "a>=1"
  if a<=1 println "a<=1"
  if a!=0 println "a!=0"
      
end

