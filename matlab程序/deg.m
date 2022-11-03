% clear all 
% This program designing by Deng Tang
function value = deg(f,n)
% truthtable2polynomial(f,n)
%%%%%%%%%%%%%%%%%%% Ö÷³ÌÐò
if f==zeros(2^n,1)
    value=0;
else
ANF=mod(abs(NNF(f,n)),2);
B=find(ANF==1)-1; 
for i=1:length(B);
    B(i)=sum(de2bi(B(i),n));
end
value=max(B);
end
    

