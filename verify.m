KLine := recformat<asq,bsq,Asq,Bsq>;

function KummerLine(asq,bsq)
	K := rec<KLine | asq := asq, bsq := bsq>;
	Asq := asq + bsq;
	Bsq := asq - bsq;
	K`Asq := Asq;
	K`Bsq := Bsq;
	return K;
end function;

function MultKL(p, k, K)
	n := Reverse(Intseq(k,2)); //print n;

	asq := K`asq; bsq := K`bsq; Asq := K`Asq; Bsq := K`Bsq;
	xsq := p[1]; zsq := p[2];

	x1 := xsq; z1 := zsq;
	x0 := Bsq*(x1+z1)^2;	//print "x0 ",(x1+z1);
	z0 := Asq*(x1-z1)^2;	//print "z0 ",(x1-z1);
	x2 := bsq*(x0+z0)^2;	//print "x2 ",x2;
	z2 := asq*(x0-z0)^2;	//print "z2 ",z2;
	i := 2;
	while IsDefined(n,i)  do
		if n[i] eq 0 then
			x0 := Bsq*(x1+z1)^2;
			z0 := Asq*(x1-z1)^2;
			x3 := bsq*(x0+z0)^2;
			z3 := asq*(x0-z0)^2;
			x0 := Bsq*(x1+z1)*(x2+z2);
			z0 := Asq*(x1-z1)*(x2-z2);
			x4 := zsq*(x0+z0)^2;
			z4 := xsq*(x0-z0)^2;
			x1 := x3; z1 := z3; x2 := x4; z2 := z4; 
		end if;
		if n[i] eq 1 then
			x0 := Bsq*(x1+z1)*(x2+z2);
			z0 := Asq*(x1-z1)*(x2-z2);
			x3 := zsq*(x0+z0)^2;
			z3 := xsq*(x0-z0)^2;
			x0 := Bsq*(x2+z2)^2;
			z0 := Asq*(x2-z2)^2;
			x4 := bsq*(x0+z0)^2;
			z4 := asq*(x0-z0)^2;
			x1 := x3; z1 := z3; x2 := x4; z2 := z4; 
		end if;
		i:=i+1;
	end while;
	return [x1,z1];
end function;

function MapToKL(P, K)
	asq := K`asq; bsq := K`bsq;
	X := P[1]/P[3];
	xsq := X*bsq;
	zsq := (X-1)*asq;
	return [xsq,zsq];
end function;

function MapToEC(p, K)
	asq := K`asq; bsq := K`bsq; 
	xsq := p[1]; zsq := p[2];
	l := asq^2/(asq^2-bsq^2);
	E := EllipticCurve([0,-l-1,0,l,0]);
	den := asq*xsq - bsq*zsq;
	if den eq 0 then return E!0; end if;
	num := asq*xsq;
	X := num/den;
	ptw := Points(E,X);
	if IsEmpty(ptw) eq false then return E!ptw[1]; end if;
end function;


// prime := 2^(251)-9; Fp := GF(prime); asq := Fp!81; bsq := Fp!20;  basept := [64, 1]; ord := 452312848583266388373324160190187140049000320168872127505022858504236695257;
// prime := 2^(255)-19; Fp := GF(prime); asq := Fp!82; bsq := Fp!77;  basept := [31, 1]; ord := 4824670384888174809315457708695329493883939011885747436657444590489242149187;
prime := 2^(266)-3; Fp := GF(prime); asq := Fp!260; bsq := Fp!139; basept := [2, 1]; ord := 9880924948250982009478057387408034803478684522013484352184368596384732719002519;

K := KummerLine(asq, bsq);
l := asq^2/(asq^2-bsq^2); // print "lambda = ", l;
E := EllipticCurve([Fp| 0, -l-1, 0, l, 0]);
T2 := E![l,0,1];

P1 := Random(E); P := P1 + T2; p := MapToKL(P, K);

// k := 3;
k := Random(1,prime);
print "k = ", k;

q := MultKL(p,k,K);
Q := k*P1 + T2;
qprime := MapToKL(Q, K);
q[1]/q[2]; qprime[1]/qprime[2]; // verifies KL ladder starting from a random point on the EC

basePt := MapToEC(basept, K) + T2;
ord*basePt + T2; // verifies order of the base point on the KL
