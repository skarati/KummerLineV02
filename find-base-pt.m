p:=2^(251)-9; F:=GF(p); a2:=F!81; b2:=F!20;
p:=2^(255)-19; F:=GF(p); a2:=F!82; b2:=F!77;
p:=2^(266)-3; F:=GF(p); a2:=F!260; b2:=F!139;
mu:=a2^2/(a2^2-b2^2);
E:=EllipticCurve([F!0,-(mu+1),0,mu,0]);
ordE := #E;
facOrdE := FactoredOrder(E);
facOrdE;
maxSubgrPr := Maximum(facOrdE);
maxSubgrPr := maxSubgrPr[1];
print "max subgr sz", maxSubgrPr;
print "log of max subgr sz", Log(2,maxSubgrPr);
for i:=1 to 10000 do
        x := F!i;
        x1 := a2*x/(a2*x-b2);
        // x1 := a2*x^2/(a2*x^2-b2);
        ptw := Points(E,x1);
        if IsEmpty(ptw) eq false then
                pt := E!ptw[1];
                ordpt := Order(pt);
                if ordpt eq maxSubgrPr then
			print "basePoint = ",i;
			print pt;
			print ordpt;
			break;
                end if;
        end if;
	if i mod 1000 eq 0 then
		print i;
	end if;
end for;
