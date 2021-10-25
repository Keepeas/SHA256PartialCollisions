# SHA256PartialCollisions
Program to find partial collisions to SHA256 hashes

Requires OpenSSH

Attempts to find SHA256 collisions and keeps the maximum sized partial match.
	-A partial match is difined as any byte length that is sequentially identical, starting with the lowest significant bytes.

Goal: To find highest reasonably attainable partial matches, given the limitations of the running hardware

This program explores the Difficulty of Bitcoin mining and other cryptocurrency mining that use similar algorithms.
"Difficulty" in Bitcoin mining directly correlates to a minimum length of matching bytes to a Hashed block

Expected to run indefinitely.
