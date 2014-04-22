BEGIN {FS =  "," }
{ 
if(NR != 1){
#print NR " adding" $1 "++";
total += $1;
frame += $2;
cc += $3;
thres += $4;
moment += $5;
area += $6;
processing += $7;
idle += $8;

if($1 > 16666){
#We are taking too long! 
timeout += 1;
}
if($1 > 16950){
timeout2 += 1;
}
if($1 > 17240){
# 58hz
timeout3 += 1;
}


if($2 >= max){
max = $2;
#print max
}

}
#print $2

 }
END {

NRx = NR-1;
 print "Total: " total/NRx;
print "tmrFrame:(experimental):" frame/NRx;
 print "\n colorConversion: @" cc/NRx
print "Thresholding: @" thres/NRx " - delta: " thres/NRx-cc/NRx;
print "Moment: @" moment/NRx " - delta: " moment/NRx-thres/NRx;
print "area: @" area/NRx " - delta: " area/NRx-moment/NRx;
print "processing(total): " processing/NRx 
print "\n idle (experimental) : " idle/NRx 
print "Samples taking more than 16666 ( 60hz): " timeout;
print "Samples taking more than 16950 ( 59 Hz): " timeout2;
print "Samples taking more than 17240 ( 58 Hz): " timeout3;
print "Max: " max " ~= " 1/max*1000000 " Hz";
 } 
