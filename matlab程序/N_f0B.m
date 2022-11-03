%Divide-and-conquer butterfly algorithm to compute the walsh spectrum
function Nf = N_f0B(f,n)
a=(1-2*f);
for i=1:n
    t=2^n/2^i;
    for j=0:2:2^i-1
        a(t*j+1:t*(j+1))=a(t*j+1:t*(j+1))+a(t*(j+1)+1:t*(j+2));
        a(t*(j+1)+1:t*(j+2))=a(t*j+1:t*(j+1))-2*a(t*(j+1)+1:t*(j+2));
    end
end
Nf=a;
        
    