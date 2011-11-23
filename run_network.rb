#!/usr/bin/ruby

t = 1
while t <= 20
	command = "/home/herb/git/networks/networks " + t.to_s + " " + (t + 1).to_s + " 1"

	puts t
	puts `#{command}`

	t += 1
	t = (t * 10000).round.to_f / 10000.0
end
