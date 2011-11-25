#!/usr/bin/ruby

t = 0.3
while t < 0.4
	command = "/home/herb/git/networks/networks " + t.to_s + " " + (t + 0.001).to_s + " 0.001"

	puts t
	puts `#{command}`

	t += 0.001
	t = (t * 10000).round.to_f / 10000.0
end
