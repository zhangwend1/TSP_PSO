#include "particle.h"
#include <iostream>

Particle::Particle(double self_trust, double past_trust, double global_trust) {
    this->self_trust = self_trust;
    this->past_trust = past_trust;
    this->global_trust = global_trust;
    this->best_value = -1;
}

double Particle::move() {
    //std::cout << "Velocity: " << velocity.to_string() << std::endl;
    position += velocity;
    //std::cout << "New position: " << position.to_string() << std::endl;

    intersect_eliminate();
    betterway();

    double new_value = this->calculate_value();
    //std::cout << new_value << std::endl;
    if (new_value < this->best_value || this->best_value < 0) {
        this->best_value = new_value;
        this->best_position = position;
    }

    return this->best_value;
}

double Particle::calculate_value() {
    double value = 0;

    int node_count = this->position.nodes.size();

    for (int i = 0; i < node_count; i++) {
        Node tmp = this->position.nodes[i];
        Node tmp2;

        if (i + 1 < node_count) {
            tmp2 = this->position.nodes[i + 1];
        }
        else {
            tmp2 = this->position.nodes[0];
        }
        double tmp_val = tmp.distance_to(tmp2);
        value += tmp_val;
    }
    return value;
}

void Particle::calculate_new_velocity(Position global_best) {
    Velocity a;
    if (this->velocity.size > 0) {
        a = (this->velocity * this->self_trust);
    }
    Velocity b = ((this->best_position - this->position) * this->past_trust);
    Velocity c = ((global_best - this->position) * this->global_trust);

    //std::cout << a.to_string();
    this->velocity = Velocity(a + b + c);
}

bool Particle::is_intersect(int i, int j) {
    int nodes_num = this->position.nodes.size();
    int i_x = this->position.nodes[i].x;
    int i_xnext = this->position.nodes[i + 1].x;
    int i_y = this->position.nodes[i].y;
    int i_ynext = this->position.nodes[i + 1].y;
    int j_x = this->position.nodes[j].x;
    int j_xnext = this->position.nodes[(j + 1)% nodes_num].x;
    int j_y = this->position.nodes[j].y;
    int j_ynext = this->position.nodes[(j + 1) % nodes_num].y;
    
    //快速排斥
    if ((i_x > i_xnext ? i_x : i_xnext) < (j_x < j_xnext ? j_x : j_xnext) ||
        (j_x > j_xnext ? j_x : j_xnext) < (i_x < i_xnext ? i_x : i_xnext) ||
        (i_y > i_ynext ? i_y : i_ynext) < (j_y < j_ynext ? j_y : j_ynext) ||
        (j_y > j_ynext ? j_y : j_ynext) < (i_y < i_ynext ? i_y : i_ynext)) 
    {
        return false;
    }

    //向量计算
    int res1 = (i_xnext - i_x) * (j_y - i_y) - (i_ynext - i_y) * (j_x - i_x);
    int res2 = (i_xnext - i_x) * (j_ynext - i_y) - (i_ynext - i_y) * (j_xnext - i_x);
    int res3 = (j_x - j_xnext) * (i_y - j_ynext) - (j_y - j_ynext) * (i_x - j_xnext);
    int res4 = (j_x - j_xnext) * (i_ynext - j_ynext) - (j_y - j_ynext) * (i_xnext - j_xnext);
    if (res1 * res2 <= 0 && res3 * res4 <= 0) {
        return true;
    }
    else {
        return false;
    }
}

void Particle::intersect_eliminate() {
    for (int i = 0; i < this->position.nodes.size()-2; i++)
    {
        for (int j = i+2; j < this->position.nodes.size(); j++)
        {
            if (i == 0 && j == 49) {
                continue;
            }
            if (position.nodes[i].x==position.nodes[j].x && position.nodes[i].y == position.nodes[j].y) {
                Velocity v;
                for (int k = j-1; k >i ; k--)
                {
                    v.add_transposition(k, j);
                }
                this->position += v;
                continue;
            }
            if (position.nodes[i].x == position.nodes[(j + 1)% this->position.nodes.size()].x && position.nodes[i].y == position.nodes[(j + 1) % this->position.nodes.size()].y) {
                Velocity v;
                for (int k = j ; k > i; k--)
                {
                    v.add_transposition(k, (j + 1) % this->position.nodes.size());
                }
                this->position += v;
                continue;
            }
            if (position.nodes[i + 1].x == position.nodes[j].x && position.nodes[i + 1].y == position.nodes[j].y) {
                Velocity v;
                for (int k = i+2 ; k <= j; k++)
                {
                    v.add_transposition(i+1, k);
                }
                this->position += v;
                continue;
            }
            if (position.nodes[i + 1].x == position.nodes[(j + 1) % this->position.nodes.size()].x && position.nodes[i + 1].y == position.nodes[(j + 1) % this->position.nodes.size()].y) {
                Velocity v;
                for (int k = i + 2; k <= j; k++)
                {
                    v.add_transposition(i + 1, k);
                }
                this->position += v;
                continue;
            }
            if (is_intersect(i,j)) {
                Velocity v;
                for (int k = i+1; k <= (j + i + 1) / 2; k++) 
                {
                    v.add_transposition(k, j + i - k + 1);
                }
                this->position += v;
            }
        }
    }
}

void Particle::betterway() {
    for (int i = 0; i < this->position.nodes.size(); i++)
    {
        Node tmp = this->position.nodes[i];
        Node tmp2 = this->position.nodes[(i + 1) % this->position.nodes.size()];
        Node tmp3 = this->position.nodes[(i + 2) % this->position.nodes.size()];
        Node tmp4 = this->position.nodes[(i + 3) % this->position.nodes.size()];
        int ori = tmp.distance_to(tmp2) + tmp2.distance_to(tmp3) + tmp3.distance_to(tmp4);
        int now = tmp.distance_to(tmp3) + tmp3.distance_to(tmp2) + tmp2.distance_to(tmp4);
        if (ori > now) 
        {
            Velocity v;
            v.add_transposition((i + 1) % this->position.nodes.size(), (i + 2) % this->position.nodes.size());
            this->position += v;
        }
    }
    
}
