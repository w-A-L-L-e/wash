#!/usr/local/bin/wsbasic

begin

  foreach word in "hello little world!" seperated by " "
    println "'", word ,"'"

  foreach word in "hello little world!" seperated by " "
  begin
    if word!="little"
      print word+" "
  end
  println ""
end

