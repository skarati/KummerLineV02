nthreads=80
lo=$1
incr=$((($2-$1+1)/$nthreads))
nbigsteps=$((($2-$1+1)-($nthreads*$incr)))
hi=$(($lo+$incr))
for((i=1; i<=$nbigsteps; i++))
do
        hi=$(($lo+$incr))
        file="results-"$1"-"$2"-"$lo"-"$hi"-"$i".dat"
        nohup magma strlo:=$lo strhi:=$hi Kummer2519.m >> $file &
        #echo $file
        lo=$(($hi+1))
done
for((i=$(($nbigsteps+1)); i<=$nthreads; i++))
do
        hi=$(($lo+$incr-1))
        file="results-"$1"-"$2"-"$lo"-"$hi"-"$i".dat"
        nohup magma strlo:=$lo strhi:=$hi Kummer2519.mag >> $file &
        #echo $file
        lo=$(($hi+1))

done
