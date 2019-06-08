#include <assert.h>
#include <stdlib.h>
#include "sway/commands.h"
#include "sway/tree/view.h"
#include "log.h"

static bool parse_opacity(const char *opacity, float *current_opacity) {
	char *err;
	float val = strtof(opacity, &err);
	if (val < -1 || val > 1 || *err) {
		return false;
	}

	if (opacity[0] != '-' && opacity[0] != '+') {
		*current_opacity = val;
		return true;
	}

	*current_opacity += val;
	if (*current_opacity < 0) {
		*current_opacity = 0;
	} else if (*current_opacity > 1) {
		*current_opacity = 1;
	}
	return true;
}

struct cmd_results *cmd_opacity(int argc, char **argv) {
	struct cmd_results *error = NULL;
	if ((error = checkarg(argc, "opacity", EXPECTED_EQUAL_TO, 1))) {
		return error;
	}

	struct sway_container *con = config->handler_context.container;

	if (con == NULL) {
		return cmd_results_new(CMD_FAILURE, "No current container");
	}

	float opacity = con->alpha;

	if (!parse_opacity(argv[0], &opacity)) {
		return cmd_results_new(CMD_INVALID,
				"Invalid value (expected -1..1): %s", argv[0]);
	}

	con->alpha = opacity;
	container_damage_whole(con);

	return cmd_results_new(CMD_SUCCESS, NULL);
}
