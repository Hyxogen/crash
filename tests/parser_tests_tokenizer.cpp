#include <deque>

extern "C" {
	#include "minishell.h"
}

static std::deque<t_token> g_tokens;

int	tk_tokenize(t_tokenizer *tk, t_token *token) {
	(void)tk;
	if (g_tokens.size() == 0)
		return (0);
	*token = g_tokens.front();
	g_tokens.pop_front();
	return (1);
}

void add_token(t_token token) {
	g_tokens.push_back(token);
}

void add_token(t_token_type type, t_token_id id, const char *str) {
	t_token token;

	token.type = type;
	token.id = id;
	token.str = str;
	add_token(token);
}

void clear_tokens() {
	g_tokens.clear();
}