#define _GNU_SOURCE

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"
#include "day_base/struct/deque.h"
#include "day_base/struct/hashmap.h"

typedef unsigned char card_t;
typedef unsigned int score_t;
typedef card_t starting_hand_t[50];
typedef card_t flat_hands_t[51];

static void deal_hands(starting_hand_t *starting_hands, deque_t *deques, card_t **hands, size_t *deck_lens) {
	for (size_t player=0; player<2; player++) {
		for (size_t i=0; i<deck_lens[player]; i++) {
			size_t idx = deque_add(&deques[player], DEQUE_BACK);
			hands[player][idx] = starting_hands[player][i];
		}
	}
}

static void flatten(flat_hands_t flat_hands, deque_t *deques, card_t **hands) {
	memset(flat_hands, 0, sizeof (flat_hands_t));
	for (size_t i=0; i<deque_len(&deques[0]); i++) {
		size_t idx = deque_peek(&deques[0], i, DEQUE_FRONT);
		flat_hands[i] = hands[0][idx];
	}
	for (size_t i=0; i<deque_len(&deques[1]); i++) {
		size_t idx = deque_peek(&deques[1], i, DEQUE_FRONT);
		flat_hands[50-i] = hands[1][idx];
	}
}

static size_t play_combat(deque_t *deques, card_t **hands, int recursive) {
	hashmap_t played;
	flat_hands_t flat_hands;
	card_t cards[2];
	size_t idx_one, idx_two, winner_idx;
	hashmap_init(&played, NULL, 0, 128);
	while (deque_len(&deques[0]) > 0 && deque_len(&deques[1]) > 0) {
		flatten(flat_hands, deques, hands);
		if (NULL != hashmap_lookup(&played, flat_hands, sizeof (flat_hands_t))) break;
		hashmap_add(&played, flat_hands, sizeof (flat_hands_t));
		idx_one = deque_remove(&deques[0], DEQUE_FRONT);
		cards[0] = hands[0][idx_one];
		idx_two = deque_remove(&deques[1], DEQUE_FRONT);
		cards[1] = hands[1][idx_two];
		if (recursive && cards[0] <= deque_len(&deques[0]) && cards[1] <= deque_len(&deques[1])) {
			deque_t new_deques[2];
			card_t *new_hands[2];
			deque_init(&new_deques[0], &new_hands[0], sizeof (card_t));
			deque_init(&new_deques[1], &new_hands[1], sizeof (card_t));
			for (size_t player=0; player<2; player++) {
				for (size_t i=0; i<cards[player]; i++) {
					size_t peek_idx = deque_peek(&deques[player], i, DEQUE_FRONT);
					size_t idx = deque_add(&new_deques[player], DEQUE_BACK);
					new_hands[player][idx] = hands[player][peek_idx];
				}
			}
			winner_idx = play_combat(new_deques, new_hands, recursive);
			deque_free(&new_deques[0]);
			deque_free(&new_deques[1]);
		} else {
			winner_idx = cards[0] > cards[1] ? 0 : 1;
		}
		idx_one = deque_add(&deques[winner_idx], DEQUE_BACK);
		hands[winner_idx][idx_one] = cards[winner_idx];
		idx_two = deque_add(&deques[winner_idx], DEQUE_BACK);
		hands[winner_idx][idx_two] = cards[1-winner_idx];
	}
	hashmap_free(&played);
	if (deque_len(&deques[0]) == 0) return 1;
	else return 0;
}

static score_t score_combat(size_t winner_idx, deque_t *deques, card_t **hands) {
	score_t score = 0;
	deque_t *winner_deque = &deques[winner_idx];
	card_t *winner_hand = hands[winner_idx];
	for (size_t i=deque_len(winner_deque); i>0; i--) {
		size_t idx = deque_remove(winner_deque, DEQUE_FRONT);
		score += i * winner_hand[idx];
	}
	return score;
}

void day_result_compute(char *arg, day_result_t *res, FILE *in) {
	char line[1024];
	size_t deck_lens[2] = {};
	starting_hand_t starting_hands[2];
	size_t curr_player;

	while (fgets(line, sizeof (line), in) != NULL) {
		if (strncmp(line, "Player 1:", 9) == 0) {
			curr_player = 0;
		} else if (strncmp(line, "Player 2:", 9) == 0) {
			curr_player = 1;
		} else if (strspn(line, "0123456789") > 0) {
			starting_hands[curr_player][deck_lens[curr_player]++] = atoi(line);
		}
	}

	// part 1
	{
		deque_t deques[2];
		card_t *hands[2];
		deque_init(&deques[0], &hands[0], sizeof (card_t));
		deque_init(&deques[1], &hands[1], sizeof (card_t));
		deal_hands(starting_hands, deques, hands, deck_lens);
		size_t winner_idx = play_combat(deques, hands, 0);
		res->part1 = score_combat(winner_idx, deques, hands);
		deque_free(&deques[0]);
		deque_free(&deques[1]);
	}

	// part 2
	{
		deque_t deques[2];
		card_t *hands[2];
		deque_init(&deques[0], &hands[0], sizeof (card_t));
		deque_init(&deques[1], &hands[1], sizeof (card_t));
		deal_hands(starting_hands, deques, hands, deck_lens);
		size_t winner_idx = play_combat(deques, hands, 1);
		res->part2 = score_combat(winner_idx, deques, hands);
		deque_free(&deques[0]);
		deque_free(&deques[1]);
	}

}

day_check_t *day_check_provide() {
	static day_check_t checks[] = {
		{"example", {306, 291}},
		{"example2", {105, 105}},
		{DAY_INPUT, {33561, 34594}},
	};
	return checks;
}
