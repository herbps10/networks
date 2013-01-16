#!/usr/bin/ruby

t = 0.025
while t < 0.03
	command = "/home/herb/git/networks/networks " + t.to_s + " " + (t + 0.1).to_s + " 0.1"

	puts t
	puts `#{command}`

	t += 0.1
	t = (t * 10000).round.to_f / 10000.0
end

