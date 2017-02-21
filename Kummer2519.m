lo := StringToInteger(strlo);
hi := StringToInteger(strhi);
p := 2^251-9;
F := GF(p);
for a:= lo to hi do
	for b:= 1 to (a-1) do
		a2 := F!a; b2 := F!b;
		if IsSquare(F!a2) eq true and IsSquare(F!b2) eq true then // enforcing a^2 and b^2 to have square roots in F
			print "a^2 = ",a2, " b^2 = ",b2;	
			lambda := a2^2/(a2^2-b2^2);
			E := EllipticCurve([F!0,-(lambda+1),0,lambda,0]);	
			ordE := #E;						
			facOrdE := FactoredOrder(E);				
			maxSubgrPr := Maximum(facOrdE);				
			maxSubgrPr := maxSubgrPr[1];				
			szMaxSubgrPr := Log(2,maxSubgrPr);			
			h := ordE/maxSubgrPr;					
			traceE := p+1-ordE;					
			// if h le 8 then
			if szMaxSubgrPr ge 247 then
				print "Checking for twist secure";
				ordTwist := p+1+traceE;					
				facOrdTwist := Factorization(ordTwist);			
				maxSubgrPrT := Maximum(facOrdTwist);				
				maxSubgrPrT := maxSubgrPrT[1];				
				szMaxSubgrPrT := Log(2,maxSubgrPrT);	
				hT := 	ordTwist/maxSubgrPrT;
				// if hT le 8 then
				if szMaxSubgrPrT ge 247 then
					print "Success";
					print "lambda = ",lambda;
					print "E = ", E;
					print "ordE = ", ordE;
					print "facOrdE = ", facOrdE;
					print "maxSubgrPr = ", maxSubgrPr;
					print "szMaxSubgrPr = ", szMaxSubgrPr;
					print "h = ",h;
					print "traceE = ",traceE;
					print "ordTwist = ",ordTwist;
					print "facOrdTwist = ",facOrdTwist;
					print "maxSubgrPrT = " ,maxSubgrPrT;
					print "szMaxSubgrPrT = ",szMaxSubgrPrT;
					print "hT = ",hT;
				else	print "Failure";
					print " "; print " ";
				end if;
			else 	print "Failure";
				print " "; print " ";
			end if;
			print "**********************************************";
		end if;
	end for;
end for;
print "Completed";
