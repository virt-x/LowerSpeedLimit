# Lower Speed Limit

Vanilla Skyrim handles the effect of the 'speedmult' actor value on a character's speed in a bit of an odd way; values of 0 act as the default value of 100, and negative values act as postive ones.

This SKSE plugin enforces a minimum effective value for speedmults, preventing these behaviors which can crop up often if you stack multiple slowing effects.
