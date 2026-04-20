# Fortify Attribute Fix

Vanilla Skyrim handles damage to actor values not as a current value, but as a negative modifier that gets added to your total. Because of this behavior, fortify/ravage health/magicka/stamina effects always update your current value as well whenever applied/removed. Infamously, this can cause you to die when a fortify health effect expires if your damage modifier exceeds the total of your updated current health.

This SKSE plugin makes damage modifiers to the three attributes get updated with an inverse change to any adjustments to the maximum values, preserving current values (and preventing issues like dying to fortify health potions.)
