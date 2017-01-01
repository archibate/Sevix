def d_conn
target remote localhost:1234
end
def d_qq
kill
quit
end
def qq
d_qq
end

file kernel.ko
d_conn
#break __kernel_entry
#break _cpu_halt
break start_kernel
continue
