BEGIN {FS =  "," }
{ 

if(NR != 1){
total += $1;
frame += $2;
cc += $3;
thres += $4;
moment += $5;
area += $6;
}
#print $2

 }
END { print "Total: " total/NR;
print "getFrame: @" frame/NR;
 print "colorConversion: @" cc/NR " - delta: " cc/NR-frame/NR;
print "Thresholding: @" thres/NR " - delta: " thres/NR-cc/NR;
print "Moment: @" moment/NR " - delta: " moment/NR-thres/NR;
print "area: @" area/NR " - delta: " area/NR-moment/NR;
 } 
