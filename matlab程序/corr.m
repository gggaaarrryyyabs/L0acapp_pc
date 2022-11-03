%correlation of two boolean functions
%for example: f1 = x1x2x3+x5x4, f2=x1x2+x4+x3x5
global n;
% n is the number of variables of the boolean functions
n = 4;
% x is the binary matrix of 0,1,2,...,2^n-1, such that 
%x5 0,0,0,0,0,...,1
%x4 0,0,0,0,0,...,1
%x3 0,0,0,0,1,...,1
%x2 0,0,1,1,0,...,1
%x1 0,1,0,1,0,...,1

x = int2bit(0:2^n-1,n);
%% f1 = x1x2x3+x5x4, f1truthtable is the truthtable of f1, 
% the value of truthtable consists of ***{0,1}***
%f1truthtable = bitxor(x(5,:).*x(4,:).*x(3,:),x(1,:).*x(2,:));
% using f=x1x2x3+x1x4+x2 to test
f1truthtable = [0 0 1 1 0 0 1 0 0 1 1 0 0 1 1 1];
%% f2=.....
%f2truthtable = bitxor(bitxor(x(5,:).*x(4,:),x(2,:)),x(3,:).*x(1,:));
f2truthtable = x(3,:);
%%
result = zeros(1,2^n);

for delta_a = 0:2^n-1
    for x = 0:2^n-1
        result(delta_a+1) = result(delta_a+1) + (-1)^bitxor(f1truthtable(x+1),f2truthtable(bitxor(x,delta_a)+1));
    end
end
f1truthtable
f2truthtable
result

