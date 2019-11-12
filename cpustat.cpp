#include <algorithm>
#include <stdio.h>
#include <unistd.h>

class ProcStat {
private:

	struct cpu_stat {
		unsigned long user = 0;
		unsigned long nice = 0;
		unsigned long sys = 0;
		unsigned long idle = 0;
		unsigned long iowait = 0;
		unsigned long irq = 0;
		unsigned long softirq = 0;
	};

	cpu_stat last_stat_;
	cpu_stat curr_stat_;

	static bool fetch(struct cpu_stat *out)
	{
		bool ok = false;
		FILE *fp = fopen("/proc/stat", "r");
		if (fp) {
			int n = fscanf(fp, "cpu %ld %ld %ld %ld %ld %ld %ld", &out->user, &out->nice, &out->sys, &out->idle, &out->iowait, &out->irq, &out->softirq);
			if (n == 7) {
				ok = true;
			}
		}
		fclose(fp);
		return ok;
	}

	static double calc(struct cpu_stat const &last, struct cpu_stat const &curr)
	{
		double t = (curr.user + curr.nice + curr.sys + curr.idle + curr.iowait + curr.irq + curr.softirq);
		t -= (last.user + last.nice + last.sys + last.idle + last.iowait + last.irq + last.softirq);
		if (t == 0) return 0;
		t = ((curr.user + curr.nice + curr.sys) - (last.user + last.nice + last.sys)) / t;
		t = std::max(0.0, std::min(1.0, t));
		return t;
	}

public:

	void update()
	{
		last_stat_ = curr_stat_;
		fetch(&curr_stat_);
	}

	void reset()
	{
		update();
		last_stat_ = curr_stat_;
	}

	double get() const
	{
		return calc(last_stat_, curr_stat_);
	}
};

int main(int argc, char * const argv[])
{
	ProcStat procstat;
	procstat.reset();
	while (1) {
		sleep(1);
		procstat.update();
		double percent = procstat.get() * 100;
		printf("%.2f%%\n", percent);
	}
	return 0;
}

