#!/bin/bash

case "$1" in
        base)
			./obj/test_A.out
			./obj/test_B.out
            ;;
         
        pwm)
			./obj/test_blade.out
			./obj/test_heartbeats.out
			./obj/test_irda_ack.out
			./obj/test_irda_dummy.out
			./obj/test_irda_pic.out
			./obj/test_pwm_functions.out
            ;;
         
         clean)
			rm -rf ./base/obj
			rm -rf ./pwm/obj
			;;
        *)
            echo $"Usage: $0 {base|pwm|clean}"
            exit 1
 
esac
