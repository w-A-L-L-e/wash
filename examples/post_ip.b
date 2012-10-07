#!/usr/local/bin/wsbasic

begin
  pass = "change this"
  url  = "http://win-www.uia.ac.be/u/wschrep/ip-post/set_ip.php"
  
  cmd     = "echo \"pass="+pass+"\" | lynx -post_data \""+url+"\""
  result  = run( cmd )
  
  test    = substr( result, 5,6 )
  
  if test == "OK" 
    println "post successfull"
  else
    println "post failed: ",result
  
end
