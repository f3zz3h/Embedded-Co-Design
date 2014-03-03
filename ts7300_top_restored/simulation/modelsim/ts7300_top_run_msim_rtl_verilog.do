transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+/home/luke/embedded-co-design/ts7300_top_restored {/home/luke/embedded-co-design/ts7300_top_restored/pll.v}
vlog -vlog01compat -work work +incdir+/home/luke/embedded-co-design/ts7300_top_restored {/home/luke/embedded-co-design/ts7300_top_restored/ts7300_top.v}
vlog -vlog01compat -work work +incdir+/home/luke/embedded-co-design/ts7300_top_restored {/home/luke/embedded-co-design/ts7300_top_restored/wb32_bridge.v}
vcom -93 -work work {/home/luke/embedded-co-design/ts7300_top_restored/ts_7300_usercore.vhd}
vcom -93 -work work {/home/luke/embedded-co-design/ts7300_top_restored/pwm.vhd}

