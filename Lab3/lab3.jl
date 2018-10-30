using DataFrames
using Statistics
using Plots
using Polynomials
using CSV

# mnożenie macierzy - wersja naiwna
function naive_multiplication(A,B)
C=zeros(Float64,size(A,1),size(B,2))
  for i=1:size(A,1)
    for j=1:size(B,2)
        for k=1:size(A,2)
            C[i,j]=C[i,j]+A[i,k]*B[k,j]
        end
    end
end
C
end

function better_multiplication( A,B )
C=zeros(Float64,size(A,1),size(B,2))
  for j=1:size(A,1)
    for k=1:size(B,2)
        for i=1:size(A,2)
            C[i,j]=C[i,j]+A[i,k]*B[k,j]
        end
    end
end
C
end

function BLAS(A, B)
A*B
end

#kompilacja
naive_multiplication(rand(1, 1), rand(1, 1))
better_multiplication(rand(1, 1), rand(1, 1))
BLAS(rand(1, 1), rand(1, 1))

#dane
function measures(init, step, ms, max)
    Iterators.flatten([x for x in init:step:max, y in 1:1:ms])
end

init=10
inc=10
ms=10
maks=500

data=DataFrame([collect(measures(init, inc, ms, maks)),
collect([@elapsed naive_multiplication(rand(x, x), rand(x, x)) for x in measures(init, inc, ms, maks)]),
collect([@elapsed better_multiplication(rand(x, x), rand(x, x)) for x in measures(init, inc, ms, maks)]),
collect([@elapsed BLAS(rand(x, x), rand(x, x)) for x in measures(init, inc, ms, maks)])],
[:size, :naive, :better, :BLAS])

bymean1=by(data, 1, df -> mean(df[2]))
bymean2=by(data, 1, df -> mean(df[3]))
bymean3=by(data, 1, df -> mean(df[4]))
byvar1=by(data, 1, df -> var(df[2]))
byvar2=by(data, 1, df -> var(df[3]))
byvar3=by(data, 1, df -> var(df[4]))

naive=join(byvar1, bymean1, on = :size)
better=join(byvar2, bymean2, on = :size)
blas=join(byvar3, bymean3, on = :size)

scatter(legend=:left, title="naive/better/blas in Julia", xlabel="rozmiar", ylabel="sredni czas", naive[1],
naive[3], yerr=naive[2], label="naive Julia")

scatter!(title="naive/better/blas", xlabel="rozmiar", ylabel="sredni czas", better[1],
better[3], yerr=better[2], colour=[:red], label="better Julia")

scatter!(title="naive/better/blas", xlabel="rozmiar", ylabel="sredni czas", blas[1],
blas[3], yerr=blas[2], colour=[:green], label="blas Julia")

plot!(naive[1], polyval(polyfit(naive[1], naive[3], 3), naive[1]), label="fit naive Julia")
plot!(better[1], polyval(polyfit(better[1], better[3], 3), better[1]), label="fit better Julia")
plot!(blas[1], polyval(polyfit(blas[1], blas[3], 3), blas[1]), label="fit blas Julia")

#from C
filename="/home/mateusz/Documents/Semester V/MOwNiTStudyCourse2018/Lab3/C/cmake-build-debug/data.csv"
dataC=CSV.read(filename, delim=";")

bymean1C=by(dataC, 1, df -> mean(df[2]))
bymean2C=by(dataC, 1, df -> mean(df[3]))
bymean3C=by(dataC, 1, df -> mean(df[4]))
byvar1C=by(dataC, 1, df -> var(df[2]))
byvar2C=by(dataC, 1, df -> var(df[3]))
byvar3C=by(dataC, 1, df -> var(df[4]))

naiveC=join(byvar1C, bymean1C, on = :size)
betterC=join(byvar2C, bymean2C, on = :size)
blasC=join(byvar3C, bymean3C, on = :size)

scatter!(title="naive/better/blas in C", xlabel="rozmiar", ylabel="sredni czas", naiveC[1],
naiveC[3], yerr=naiveC[2], label="naive C", colour=[:pink])

scatter!(title="naive/better/blas", xlabel="rozmiar", ylabel="sredni czas", betterC[1],
betterC[3], yerr=betterC[2], colour=[:brown], label="better C")

scatter!(title="naive/better/blas", xlabel="rozmiar", ylabel="sredni czas", blasC[1],
blasC[3], yerr=blasC[2], colour=[:magenta], label="blas C")

plot!(naiveC[1], polyval(polyfit(naiveC[1], naiveC[3], 3), naiveC[1]), label="fit naive C")
plot!(betterC[1], polyval(polyfit(betterC[1], betterC[3], 3), betterC[1]), label="fit better C")
plot!(blasC[1], polyval(polyfit(blasC[1], blasC[3], 3), blasC[1]), label="fit blas C")
