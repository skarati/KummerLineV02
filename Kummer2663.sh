nthreads=80
lo=$1
incr=$((($2-$1)/$nthreads))
hi=$(($lo+$incr))
for((i=1; i<=$nthreads; i++))
do 
	file="results-"$1"-"$2"-"$i".dat"
	nohup magma strlo:=$lo strhi:=$hi Kummer2663.m >> $file & 
	#echo $file
	lo=$(($hi+1))
	hi=$(($lo+$incr))
done
