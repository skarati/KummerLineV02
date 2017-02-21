p := 2^266-3;
F := GF(p);
Z := Integers();
a2 := F!260;	print "a^2: ", a2;
b2 := F!139;	print "b^2: ", b2;
lambda := a2^2/(a2^2-b2^2);	print "lambda: ",lambda;
beta := (lambda+1)/3;		print "beta: ",beta;
ap := lambda -3*beta^2;		print "ap: ",ap;
bp := beta^3 + ap*beta;		print "bp: ",bp;
E := EllipticCurve([F!ap,bp]);	print E;
ord := #E;			print "Order of E: ",ord;
fac := FactoredOrder(E);	print "Factors of #E: ", fac;
l := Maximum(fac)[1];		print "Max prime divisor, l : ",l;
F1 := GF(l);
pr := IsPrime(l);		print "Is l prime: ",pr;
size := Log(2,l);		print "Size of l: ",size;
h := ord/l;			print "Cofactor of E, h: ",h;
t := p+1-ord;			print "Trace, t: ",t;
twist_ord := p+1+t;		print "Order of nontrivial twist: ",twist_ord;
fac_t := Factorization(twist_ord);	print "Factors of twist: ", fac;	
max_t := Maximum(fac_t)[1];	print "Max prime divisor of twist, lt : ",max_t;
pr_t := IsPrime(max_t);		print "Is lt prime: ",pr_t;
size_t := Log(2,max_t);		print "Size of lt: ",size_t;
h_t := twist_ord/max_t;		print "Cofactor of twist, ht: ",h_t;

pList:=[p,ord,twist_ord];
pList:=Append(pList,AbsoluteValue(t^2-4*p));
pL:=[];
i:=1;
while IsDefined(pList, i) do
	q:= pList[i];
	if q ne 1 then
		fac:= Factorization(q);
		j:=1;
		while IsDefined(fac, j) do
			pL:=Append(pL, fac[j,1]);
			j:=j+1;
		end while;
	end if;
	i:=i+1;
end while;

i:=1;
while IsDefined(pL, i) do
	q:= pL[i]-1;
	if q ne 1 then
		fac:= Factorization(q);
		j:=1;
		while IsDefined(fac, j) do
			pL:=Append(pL, fac[j,1]);
			j:=j+1;
		end while;
	end if;
	i:=i+1;
end while;
pL:= Sort(pL);

V:=[pL[1]];
i:=2;
j:=1;
while IsDefined(pL, i) do
	if pL[i] ne V[j] then
		V:=Append(V,pL[i]);
		j:=j+1;
	end if; 	
	i:=i+1;	
end while;

gcdlp := GCD(l,p);		print "GCD of l and p :", gcdlp;
u := F1!(p mod l);
print u;
d := (l-1);
i := 1;
while IsDefined(V, i) do
	v := V[i];
      	while (d mod v) eq 0 do
		d := d div v;
	end while;
	i := i+1;
end while;
i := 1;
if d eq 1 then
	d := l-1;
	while IsDefined(V, i) do
		v := V[i];
		while (d mod v) eq 0 do
			if u^(d div v) ne 1 then
				break;
			end if;
			d := d div v;
		end while;
		i := i+1;
	end while;
end if;

print "embedding degree, d : ",d;
print "=(l-1)/",(l-1)/d;
print "Is l^2>16p";
if l^2 ge 16*p then
	print "= Yes, l^2-16*p = ",l^2-16*p;
end if;
Disc := t^2 - 4*p;
print "Discriminant : ", Disc;
i := 1;
while IsDefined(V, i) do
	v := V[i];
	while (Disc mod v^2) eq 0 do
		Disc := Disc div v^2;
	end while;
	i := i+1;
end while;
facD := [];
checkD := 1;
sizeD := Log(2,Abs(Disc));
i := 1;
while IsDefined(V, i) do
	v := V[i];
	if (Disc mod v) eq 0 then
		facD := Append(facD, v);
		checkD := checkD*v;
	end if;
	i := i+1;
end while;
if checkD eq (-1)*Disc then
	print "Squarefree Discriminant : ", Disc;
	if (Disc mod 4) ne 1 then
		Disc := Disc*4;
	end if;
	print "Factorization of Squarefree Discriminant: ",facD;
	sizeD := Log(2,Abs(Disc));
	print "Size of discriminant : ", sizeD;
end if;


pi4 := 0.78539816339744830961566084581987572105;
rho := Log(4,pi4*l);
print "rho security : ",rho;

twistl := 0;
d := p+1+t;
i := 1;
while IsDefined(V, i) do
	v := V[i];
	while (d mod v) eq 0 do
		d := d div v;
	end while;
	i := i+1;
end while;
if d eq 1 then
	d := p+1+t;
	i := 1;
	while IsDefined(V, i) do
		v := V[i];
		if (d mod v) eq 0 then
			if v gt twistl then
				twistl := v;
			end if;
		end if;
		i := i+1;
	end while;
end if;
print "twistl : ", twistl;
F1 := GF(twistl);
gcdlp := GCD(twistl,p);		print "GCD of twistl and p :", gcdlp;
u := F1!(p mod twistl);
print u;
d := (twistl-1);
i := 1;
while IsDefined(V, i) do
	v := V[i];
      	while (d mod v) eq 0 do
		d := d div v;
	end while;
	i := i+1;
end while;
i := 1;
if d eq 1 then
	d := twistl-1;
	while IsDefined(V, i) do
		v := V[i];
		while (d mod v) eq 0 do
			if u^(d div v) ne 1 then
				break;
			end if;
			d := d div v;
		end while;
		i := i+1;
	end while;
end if;

print "embedding degree of twist, twistd : ",d;
print "=(twistl-1)/",(twistl-1)/d;
rho := Log(4,pi4*twistl);
print "twist rho security : ",rho;
precomp := 0;
joint := l;
i := 1;
while IsDefined(V, i) do
	v := V[i];
	d1 := p+1-t;
	d2 := p+1+t;
      	while (d1 mod v) eq 0 or (d2 mod v) eq 0 do
		if (d1 mod v) eq 0 then 
			d1 := d1 div v;
		end if;
		if (d2 mod v) eq 0 then 
			d2 := d2 div v;
		end if;
		if v + Sqrt(RealField(300)!(pi4)*joint/v) lt Sqrt(RealField(300)!(pi4)*joint) then
			precomp := precomp + v;
			joint := joint div v;
		end if;
	end while;
	i := i+1;
end while;
rho := Log(2,precomp + Sqrt(RealField(300)!(pi4) * joint));
print "Joint rho security : ",rho;


elliptic := F!(4*ap^3+27*bp^2);
print "elliptic : ", elliptic;

if (p+1-t) mod 2 eq 0 then
	if bp ne 0 then
		print "indistinguishability = True";
    	end if;
end if;
